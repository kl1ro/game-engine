add_executable(obj_test
	src/camera.cpp
	src/config.cpp
	src/entry.cpp
	src/file.cpp
	src/globals.cpp
	src/keyboard.cpp
	src/mouse.cpp
	src/opengl-helpers.cpp
	src/shader.cpp
	src/time.cpp
	src/render.cpp
)

target_link_libraries(obj_test PRIVATE
	OpenGL::GL
	glfw
	nlohmann_json::nlohmann_json
	glm::glm
	tinyobjloader
)

target_include_directories(obj_test PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${stb_SOURCE_DIR}
)

set_target_properties(obj_test PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist
)

target_compile_definitions(obj_test PRIVATE GL_GLEXT_PROTOTYPES)

add_custom_command(TARGET obj_test POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/tests/obj-test/shaders
    ${CMAKE_BINARY_DIR}/dist/shaders
	COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/tests/obj-test/assets
    ${CMAKE_BINARY_DIR}/dist/assets
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/tests/obj-test/config.json
    ${CMAKE_BINARY_DIR}/dist/config.json
  COMMENT "Copying shaders and config..."
)
