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

add_custom_command(TARGET opengl_test POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/tests/opengl-test/shaders
    ${CMAKE_BINARY_DIR}/dist/shaders
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/src/tests/opengl-test/config.json
    ${CMAKE_BINARY_DIR}/dist/config.json
  COMMENT "Copying shaders and config..."
)