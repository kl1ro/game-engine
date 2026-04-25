#define TINYOBJLOADER_IMPLEMENTATION
#include "file.hpp"
#include "globals.hpp"

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

	if (!err.empty()) std::cerr << "OBJ error: " << err << "\n";
	if (!success) throw std::runtime_error("Failed to load OBJ: " + path);

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

void loadMesh(
	const std::vector<tinyobj::shape_t>& shapes,
	const tinyobj::attrib_t& attrib
) {
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
				float u  = idx.texcoord_index >= 0 ? attrib.texcoords[2 * idx.texcoord_index + 0] : 0.0f;
				float v_ = idx.texcoord_index >= 0 ? attrib.texcoords[2 * idx.texcoord_index + 1] : 0.0f;

				submesh.vertices.insert(submesh.vertices.end(), {vx, vy, vz, u, v_});
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

	Globals::meshes.push_back(mesh);
}
