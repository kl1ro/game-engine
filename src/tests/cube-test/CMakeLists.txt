add_executable(cube_test
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
)

target_link_libraries(cube_test PRIVATE
	OpenGL::GL
	glfw
	nlohmann_json::nlohmann_json
	glm::glm
)

target_include_directories(cube_test PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${stb_SOURCE_DIR}
)

set_target_properties(cube_test PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist
)

add_custom_command(TARGET cube_test POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/tests/cube-test/shaders
    ${CMAKE_BINARY_DIR}/dist/shaders
	COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/tests/cube-test/assets
    ${CMAKE_BINARY_DIR}/dist/assets
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/src/tests/cube-test/config.json
    ${CMAKE_BINARY_DIR}/dist/config.json
  COMMENT "Copying shaders and config..."
)
