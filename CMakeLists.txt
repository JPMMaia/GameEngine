cmake_minimum_required(VERSION 3.12)

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")


include (FetchContent)

FetchContent_Declare (
	MaiaUtilities
	GIT_REPOSITORY "https://github.com/JPMMaia/Utilities.git"
	GIT_TAG "master"
)

FetchContent_GetProperties (MaiaUtilities)
if (NOT maiautilities_POPULATED)
	FetchContent_Populate (MaiaUtilities)
	add_subdirectory (${maiautilities_SOURCE_DIR} ${maiautilities_BINARY_DIR})
	list (APPEND CMAKE_MODULE_PATH "${maiautilities_SOURCE_DIR}/CMake")
endif ()


FetchContent_Declare (
	MaiaRenderer
	GIT_REPOSITORY "https://github.com/JPMMaia/Renderer.git"
	GIT_TAG "master"
)

FetchContent_GetProperties (MaiaRenderer)
if (NOT maiarenderer_POPULATED)
	FetchContent_Populate (MaiaRenderer)
	add_subdirectory (${maiarenderer_SOURCE_DIR} ${maiarenderer_BINARY_DIR})
endif ()


project(MaiaGameEngine LANGUAGES C CXX)

add_subdirectory("Source")
add_subdirectory("Test")
