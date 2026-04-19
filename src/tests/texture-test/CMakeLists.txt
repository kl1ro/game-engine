add_executable(texture_test
	src/entry.cpp
	src/file.cpp
	src/shader.cpp
	src/config.cpp
	src/opengl-helpers.cpp
)

target_link_libraries(texture_test PRIVATE
	OpenGL::GL
	glfw
	nlohmann_json::nlohmann_json
)

target_include_directories(texture_test PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${stb_SOURCE_DIR}
)

set_target_properties(texture_test PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist
)

add_custom_command(TARGET texture_test POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/tests/texture-test/shaders
    ${CMAKE_BINARY_DIR}/dist/shaders
	COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/tests/texture-test/assets
    ${CMAKE_BINARY_DIR}/dist/assets
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/src/tests/texture-test/config.json
    ${CMAKE_BINARY_DIR}/dist/config.json
  COMMENT "Copying shaders and config..."
)
