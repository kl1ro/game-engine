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
