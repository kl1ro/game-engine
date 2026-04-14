add_executable(opengl_test
	src/entry.cpp
	src/file.cpp
	src/shader.cpp
	src/config.cpp
	src/opengl-helpers.cpp
)

target_link_libraries(opengl_test PRIVATE
	OpenGL::GL
	glfw
	nlohmann_json::nlohmann_json
)

target_include_directories(opengl_test PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(opengl_test PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dist
)

file(COPY shaders/ DESTINATION ${CMAKE_BINARY_DIR}/dist/shaders)
file(COPY config.json DESTINATION ${CMAKE_BINARY_DIR}/dist)