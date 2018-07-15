project (MaiaGameEngine)

add_library (MaiaGameEngine)
add_library (Maia::GameEngine ALIAS MaiaGameEngine)

target_include_directories (MaiaGameEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../..")

set_target_properties (MaiaGameEngine PROPERTIES LINKER_LANGUAGE CXX)

target_compile_features (MaiaGameEngine PRIVATE cxx_std_17)

if (MSVC)
	target_compile_options (MaiaGameEngine PRIVATE "/permissive-")
endif ()

target_sources (MaiaGameEngine 

	PRIVATE

		"Transforms/Transform.h"
		"Transforms/Transform.cpp"
		"Transforms/Transforms_tree.h"

)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
target_include_directories (MaiaGameEngine INTERFACE "$<TARGET_PROPERTY:Eigen3::Eigen,INTERFACE_INCLUDE_DIRECTORIES>")
target_link_libraries (MaiaGameEngine PUBLIC Eigen3::Eigen)
