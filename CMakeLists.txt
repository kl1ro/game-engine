cmake_minimum_required(VERSION 3.20)

project(GameEngine LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Important for IDEs (clangd, VSCode)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -----------------------
# Dependencies
# -----------------------
include(FetchContent)
find_package(OpenGL REQUIRED)

FetchContent_Declare(
	glfw
	GIT_REPOSITORY https://github.com/glfw/glfw.git
	GIT_TAG 3.3.9
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
	json
	GIT_REPOSITORY https://github.com/nlohmann/json.git
	GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(json)

FetchContent_Declare(
	stb
	GIT_REPOSITORY https://github.com/nothings/stb.git
	GIT_TAG master
)

FetchContent_MakeAvailable(stb)

FetchContent_Declare(
	obj
	GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader
	GIT_TAG v2.0.0rc13
)

FetchContent_MakeAvailable(obj)

# -----------------------
# Custom targets
# -----------------------
add_custom_target(soft_clean
	COMMAND find . -mindepth 1 ! -name '_deps' ! -path '*/_deps/*' -delete
)

# -----------------------
# Subdirectories
# -----------------------
add_subdirectory(src/tests/opengl-test)
add_subdirectory(src/tests/quad-test)
add_subdirectory(src/tests/texture-test)
add_subdirectory(src/tests/transformation-test)
add_subdirectory(src/tests/cube-test)
add_subdirectory(src/tests/obj-test)
