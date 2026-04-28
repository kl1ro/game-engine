add_executable(quad_test
	src/entry.cpp
	src/file.cpp
	src/shader.cpp
	src/config.cpp
	src/opengl-helpers.cpp
)

target_link_libraries(quad_test PRIVATE
	OpenGL::GL
	glfw
	nlohmann_json::nlohmann_json
)

target_include_directories(quad_test PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(quad_test PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist
)

target_compile_definitions(quad_test PRIVATE GL_GLEXT_PROTOTYPES)

add_custom_command(TARGET quad_test POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/tests/quad-test/shaders
    ${CMAKE_BINARY_DIR}/dist/shaders
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/tests/quad-test/config.json
    ${CMAKE_BINARY_DIR}/dist/config.json
  COMMENT "Copying shaders and config..."
)
