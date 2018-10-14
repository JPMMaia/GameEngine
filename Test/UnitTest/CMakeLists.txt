project (MaiaGameEngineUnitTest)

add_executable (MaiaGameEngineUnitTest)
add_executable (Maia::GameEngine::UnitTest ALIAS MaiaGameEngineUnitTest)

target_compile_features (MaiaGameEngineUnitTest PRIVATE cxx_std_17)

target_link_libraries (MaiaGameEngineUnitTest PRIVATE Maia::GameEngine)

find_package (Catch2 REQUIRED)
target_link_libraries (MaiaGameEngineUnitTest PRIVATE Catch2::Catch2)

target_sources (MaiaGameEngineUnitTest 
	PRIVATE
		"main.cpp"
		"Component_group.test.cpp"
		"Entity_manager.test.cpp"
		"Memory_chunk.test.cpp"
)

include (Catch)
catch_discover_tests (MaiaGameEngineUnitTest)
