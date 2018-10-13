project (MaiaGameEngine VERSION 0.0.1)

add_library (MaiaGameEngine)
add_library (Maia::GameEngine ALIAS MaiaGameEngine)

set_target_properties(MaiaGameEngine 
	PROPERTIES 
		LINKER_LANGUAGE CXX
)

target_compile_features (MaiaGameEngine 
	PRIVATE 
		cxx_std_17
)

target_include_directories (MaiaGameEngine 
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
		$<INSTALL_INTERFACE:include/Maia/GameEngine>
)

find_package (Eigen3 REQUIRED)
target_link_libraries (MaiaGameEngine PUBLIC Eigen3::Eigen)

find_package (GSL REQUIRED)
target_link_libraries (MaiaGameEngine PUBLIC GSL::GSL)

target_sources (MaiaGameEngine 
	PRIVATE
		"Maia/GameEngine/Component_group.hpp"
		"Maia/GameEngine/Entity.hpp"
		"Maia/GameEngine/Entity_manager.hpp"
		"Maia/GameEngine/Entity_type.hpp"
		"Maia/GameEngine/Memory_chunk.hpp"
		"Maia/GameEngine/Dummy.cpp"
)


install (TARGETS MaiaGameEngine EXPORT MaiaGameEngineTargets
	LIBRARY DESTINATION "lib"
	ARCHIVE DESTINATION "lib"
	RUNTIME DESTINATION "bin"
	INCLUDES DESTINATION "include"
)
install (DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/Maia"
		DESTINATION "include"
        FILES_MATCHING PATTERN "*.hpp"
)
install (EXPORT MaiaGameEngineTargets
	FILE "MaiaGameEngineTargets.cmake"
	NAMESPACE Maia::
	DESTINATION "lib/cmake/Maia/GameEngine"
)

include (CMakePackageConfigHelpers)
write_basic_package_version_file ("${CMAKE_CURRENT_BINARY_DIR}/MaiaGameEngineConfigVersion.cmake"
	VERSION ${MaiaGameEngine_VERSION}
	COMPATIBILITY SameMajorVersion
)
install (FILES "MaiaGameEngineConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/MaiaGameEngineConfigVersion.cmake"
	DESTINATION "lib/cmake/Maia/GameEngine"
)
