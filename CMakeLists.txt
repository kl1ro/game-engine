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

FetchContent_Declare(
	imgui
	GIT_REPOSITORY https://github.com/ocornut/imgui.git
	GIT_TAG docking
)

FetchContent_MakeAvailable(imgui)

# -----------------------
# Custom targets
#
add_custom_target(soft_clean
	COMMAND find . -depth -mindepth 1 ! -name '_deps' ! -path '*/_deps/*' -delete
)

add_library(imgui STATIC
	${imgui_SOURCE_DIR}/imgui.cpp
	${imgui_SOURCE_DIR}/imgui_draw.cpp
	${imgui_SOURCE_DIR}/imgui_tables.cpp
	${imgui_SOURCE_DIR}/imgui_widgets.cpp
	${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
	${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
	${imgui_SOURCE_DIR}
	${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

# -----------------------
# Subdirectories
# -----------------------
add_subdirectory(src)
add_subdirectory(src/tests/opengl-test)
add_subdirectory(src/tests/quad-test)
add_subdirectory(src/tests/texture-test)
add_subdirectory(src/tests/transformation-test)
add_subdirectory(src/tests/cube-test)
add_subdirectory(src/tests/obj-test)
add_subdirectory(src/tests/obj-texture-test)
add_subdirectory(src/tests/lighting-test)
