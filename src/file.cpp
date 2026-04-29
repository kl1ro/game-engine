#include "file.hpp"

#include "globals.hpp"

std::string openFileDialog(const std::string& filter) {
  std::string command = "zenity --file-selection --file-filter='" + filter + " | " + filter + "'";
  FILE* f = popen(command.c_str(), "r");

  if (!f)
    return "";

  char path[1024] = {};
  fgets(path, sizeof(path), f);
  pclose(f);
  std::string result(path);

  if (!result.empty() && result.back() == '\n')
    result.pop_back();

  return result;
}

std::string readFile(const std::string& path) {
  std::ifstream file(path);

  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << path << "\n";
    std::exit(1);
  }

  std::stringstream buf;
  buf << file.rdbuf();
  return buf.str();
}

void loadObj(const std::string& path) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> tinyMaterials;
  std::string warn, err;

  std::string baseDir = std::filesystem::path(path).parent_path().string();

  bool success = tinyobj::LoadObj(
    &attrib,
    &shapes,
    &tinyMaterials,
    &warn,
    &err,
    path.c_str(),
    baseDir.c_str(),
    true
  );

  if (!err.empty())
    std::cerr << "OBJ error: " << err << "\n";
  if (!success)
    throw std::runtime_error("Failed to load OBJ: " + path);

  loadMesh(shapes, attrib);
  loadMaterials(baseDir, tinyMaterials);
}

void loadMaterials(
  const std::string baseDir,
  const std::vector<tinyobj::material_t>& tinyMaterials
) {
  for (auto& mat : tinyMaterials) {
    Material material = {};

    if (!mat.diffuse_texname.empty()) {
      std::string texPath = baseDir + "/" + mat.diffuse_texname;
      glGenTextures(1, &material.diffuseTexture);
      loadTexture(texPath, material.diffuseTexture);
    }

    material.diffuseColor = glm::vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
    Globals::materials.push_back(material);
  }
}

void loadMesh(const std::vector<tinyobj::shape_t>& shapes, const tinyobj::attrib_t& attrib) {
  Mesh mesh;
  int materialOffset = Globals::materials.size();

  for (auto& shape : shapes) {
    std::unordered_map<int, Submesh> submeshMap;
    int faceIndex = 0;

    for (int f = 0; f < (int)shape.mesh.material_ids.size(); f++) {
      int matId = shape.mesh.material_ids[f];
      Submesh& submesh = submeshMap[matId];
      submesh.materialIndex = materialOffset + matId;

      for (int v = 0; v < 3; v++) {
        tinyobj::index_t idx = shape.mesh.indices[f * 3 + v];

        float vx = attrib.vertices[3 * idx.vertex_index + 0];
        float vy = attrib.vertices[3 * idx.vertex_index + 1];
        float vz = attrib.vertices[3 * idx.vertex_index + 2];
        float u = idx.texcoord_index >= 0 ? attrib.texcoords[2 * idx.texcoord_index + 0] : 0.0f;
        float v_ = idx.texcoord_index >= 0 ? attrib.texcoords[2 * idx.texcoord_index + 1] : 0.0f;

        float nx = idx.normal_index >= 0 ? attrib.normals[3 * idx.normal_index + 0] : 0.0f;
        float ny = idx.normal_index >= 0 ? attrib.normals[3 * idx.normal_index + 1] : 0.0f;
        float nz = idx.normal_index >= 0 ? attrib.normals[3 * idx.normal_index + 2] : 1.0f;

        submesh.vertices.insert(submesh.vertices.end(), {vx, vy, vz, nx, ny, nz, u, v_});
        submesh.indices.push_back(submesh.indices.size());
      }
    }

    for (auto& [matId, submesh] : submeshMap) {
      submesh.buffers = getEmptyVertexBuffers();
      loadAttributesIntoVAO(Globals::config.triangle.attributes);
      loadVerticesIntoVBO(submesh.vertices);
      loadIndicesIntoEBO(submesh.indices);
      mesh.submeshes.push_back(submesh);
    }
  }

  Object object;
  object.mesh = mesh;
  Globals::scene.objects.push_back(object);
}

static GLuint loadGLTFTexture(const tinygltf::Model& model, int textureIndex) {
  if (textureIndex < 0)
    return 0;

  const tinygltf::Texture& tex = model.textures[textureIndex];
  const tinygltf::Image& image = model.images[tex.source];
  GLuint texId;

  glGenTextures(1, &texId);
  glBindTexture(GL_TEXTURE_2D, texId);

  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGBA,
    image.width,
    image.height,
    0,
    image.component == 3 ? GL_RGB : GL_RGBA,
    GL_UNSIGNED_BYTE,
    image.image.data()
  );

  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return texId;
}

static int loadGLTFMaterials(const tinygltf::Model& model) {
  int materialOffset = Globals::materials.size();

  for (auto& mat : model.materials) {
    Material material = {};
    auto& pbr = mat.pbrMetallicRoughness;

    material.diffuseColor =
      glm::vec3(pbr.baseColorFactor[0], pbr.baseColorFactor[1], pbr.baseColorFactor[2]);

    material.diffuseTexture = loadGLTFTexture(model, pbr.baseColorTexture.index);
    material.normalTexture = loadGLTFTexture(model, mat.normalTexture.index);
    material.emissiveTexture = loadGLTFTexture(model, mat.emissiveTexture.index);

    material.emissiveColor =
      glm::vec3(mat.emissiveFactor[0], mat.emissiveFactor[1], mat.emissiveFactor[2]);

    material.opacity = 1.0f;
    Globals::materials.push_back(material);
  }

  return materialOffset;
}

static const float* getBufferData(const tinygltf::Model& model, int accessorIndex) {
  const tinygltf::Accessor& accessor = model.accessors[accessorIndex];
  const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
  const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

  return reinterpret_cast<const float*>(
    buffer.data.data() + bufferView.byteOffset + accessor.byteOffset
  );
}

static const unsigned short* getIndexData(const tinygltf::Model& model, int accessorIndex) {
  const tinygltf::Accessor& accessor = model.accessors[accessorIndex];
  const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
  const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

  return reinterpret_cast<const unsigned short*>(
    buffer.data.data() + bufferView.byteOffset + accessor.byteOffset
  );
}

static Mesh loadGLTFMesh(const tinygltf::Model& model, int meshIndex, int materialOffset) {
  Mesh mesh;
  const tinygltf::Mesh& gltfMesh = model.meshes[meshIndex];

  for (auto& primitive : gltfMesh.primitives) {
    Submesh submesh;
    submesh.materialIndex = materialOffset + (primitive.material >= 0 ? primitive.material : 0);

    const float* positions = nullptr;
    const float* normals = nullptr;
    const float* uvs = nullptr;
    int vertexCount = 0;

    if (primitive.attributes.count("POSITION")) {
      positions = getBufferData(model, primitive.attributes.at("POSITION"));
      vertexCount = model.accessors[primitive.attributes.at("POSITION")].count;
    }

    if (primitive.attributes.count("NORMAL"))
      normals = getBufferData(model, primitive.attributes.at("NORMAL"));

    if (primitive.attributes.count("TEXCOORD_0"))
      uvs = getBufferData(model, primitive.attributes.at("TEXCOORD_0"));

    // interleave into: pos(3) + normal(3) + uv(2)
    for (int i = 0; i < vertexCount; i++) {
      submesh.vertices.push_back(positions ? positions[i * 3 + 0] : 0.0f);
      submesh.vertices.push_back(positions ? positions[i * 3 + 1] : 0.0f);
      submesh.vertices.push_back(positions ? positions[i * 3 + 2] : 0.0f);
      submesh.vertices.push_back(normals ? normals[i * 3 + 0] : 0.0f);
      submesh.vertices.push_back(normals ? normals[i * 3 + 1] : 0.0f);
      submesh.vertices.push_back(normals ? normals[i * 3 + 2] : 1.0f);
      submesh.vertices.push_back(uvs ? uvs[i * 2 + 0] : 0.0f);
      submesh.vertices.push_back(uvs ? uvs[i * 2 + 1] : 0.0f);
    }

    // indices
    if (primitive.indices >= 0) {
      const tinygltf::Accessor& idxAccessor = model.accessors[primitive.indices];
      int indexCount = idxAccessor.count;
      if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
        const unsigned short* idx = getIndexData(model, primitive.indices);
        for (int i = 0; i < indexCount; i++) submesh.indices.push_back(idx[i]);
      } else if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
        const unsigned int* idx = reinterpret_cast<const unsigned int*>(
          model.buffers[model.bufferViews[idxAccessor.bufferView].buffer].data.data()
          + model.bufferViews[idxAccessor.bufferView].byteOffset + idxAccessor.byteOffset
        );
        for (int i = 0; i < indexCount; i++) submesh.indices.push_back(idx[i]);
      }
    }

    submesh.buffers = getEmptyVertexBuffers();
    loadAttributesIntoVAO(Globals::config.triangle.attributes);
    loadVerticesIntoVBO(submesh.vertices);
    loadIndicesIntoEBO(submesh.indices);
    mesh.submeshes.push_back(submesh);
  }
  return mesh;
}

static Transform nodeToTransform(const tinygltf::Node& node) {
  Transform transform;

  if (node.translation.size() == 3)
    transform.setPosition(glm::vec3(node.translation[0], node.translation[1], node.translation[2]));

  if (node.scale.size() == 3)
    transform.setScale(glm::vec3(node.scale[0], node.scale[1], node.scale[2]));

  if (node.rotation.size() == 4) {
    glm::quat q(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);
    glm::vec3 euler = glm::degrees(glm::eulerAngles(q));
    transform.setRotation(euler);
  }

  return transform;
}

static Object loadNode(const tinygltf::Model& model, int nodeIndex, int materialOffset) {
  const tinygltf::Node& node = model.nodes[nodeIndex];

  Object object;
  object.name = node.name;
  object.transform = nodeToTransform(node);

  if (node.mesh >= 0)
    object.mesh = loadGLTFMesh(model, node.mesh, materialOffset);

  for (int childIndex : node.children)
    object.children.push_back(loadNode(model, childIndex, materialOffset));

  return object;
}

Scene loadScene(const std::string& path) {
  tinygltf::Model model;
  tinygltf::TinyGLTF loader;
  std::string err, warn;

  bool success = path.ends_with(".glb") ? loader.LoadBinaryFromFile(&model, &err, &warn, path)
                                        : loader.LoadASCIIFromFile(&model, &err, &warn, path);

  if (!warn.empty())
    std::cerr << "GLTF warning: " << warn << "\n";

  if (!err.empty())
    std::cerr << "GLTF error: " << err << "\n";

  if (!success)
    throw std::runtime_error("Failed to load GLTF: " + path);

  int materialOffset = loadGLTFMaterials(model);

  Scene scene;
  const tinygltf::Scene& gltfScene = model.scenes[model.defaultScene >= 0 ? model.defaultScene : 0];

  for (int nodeIndex : gltfScene.nodes) {
    Object node = loadNode(model, nodeIndex, materialOffset);
    scene.objects.push_back(node);
  }

  if (model.extensions.count("KHR_lights_punctual")) {
    auto& lightsJson = model.extensions.at("KHR_lights_punctual").Get("lights");

    for (int nodeIndex : gltfScene.nodes) {
      const tinygltf::Node& node = model.nodes[nodeIndex];

      if (!node.extensions.count("KHR_lights_punctual"))
        continue;

      int lightIndex = node.extensions.at("KHR_lights_punctual").Get("light").GetNumberAsInt();
      auto& lightJson = lightsJson.Get(lightIndex);
      Light light;
      light.name = node.name;

      if (node.translation.size() == 3)
        light.position = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);

      if (lightJson.Has("color")) {
        auto& c = lightJson.Get("color");
        light.color = glm::vec3(
          c.Get(0).GetNumberAsDouble(),
          c.Get(1).GetNumberAsDouble(),
          c.Get(2).GetNumberAsDouble()
        );
      }

      if (lightJson.Has("intensity"))
        light.intensity = lightJson.Get("intensity").GetNumberAsDouble();

      scene.lights.push_back(light);
    }
  }

  return scene;
}
