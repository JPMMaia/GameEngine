cmake_minimum_required (VERSION 3.11)
project (MaiaGameEngine)

add_library (MaiaGameEngine)
add_library (Maia::GameEngine ALIAS MaiaGameEngine)

set_target_properties (MaiaGameEngine PROPERTIES LINKER_LANGUAGE CXX)

target_compile_features(MaiaGameEngine PRIVATE cxx_std_17)

if (MSVC)
	target_compile_options(MaiaGameEngine PRIVATE "/permissive-")
endif ()

set_target_properties(MaiaGameEngine PROPERTIES 
	CXX_STANDARD_REQUIRED 17 
	CXX_EXTENSIONS OFF
)

target_sources (MaiaGameEngine 

	PRIVATE

		"Source/GameEngine/Transform.h"
		"Source/GameEngine/Transform.cpp"

)

target_include_directories (MaiaGameEngine PRIVATE "Source")

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
target_include_directories (MaiaGameEngine INTERFACE "$<TARGET_PROPERTY:Eigen3::Eigen,INTERFACE_INCLUDE_DIRECTORIES>")
target_link_libraries (MaiaGameEngine Eigen3::Eigen)
