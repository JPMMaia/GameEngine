project (MaiaGameEngineUnitTest)

add_executable (MaiaGameEngineUnitTest)
add_executable (Maia::GameEngine::UnitTest ALIAS MaiaGameEngineUnitTest)

target_compile_features (MaiaGameEngineUnitTest PRIVATE cxx_std_17)

target_link_libraries (MaiaGameEngineUnitTest PRIVATE Maia::GameEngine)

find_package (GTest REQUIRED)
target_link_libraries (MaiaGameEngineUnitTest PRIVATE GTest::GTest GTest::Main)

find_package (GMock REQUIRED)
target_link_libraries (MaiaGameEngineUnitTest PRIVATE GMock::GMock GMock::Main)

target_sources (MaiaGameEngineUnitTest 
	PRIVATE
		"main.cpp"
		"Transforms/Transform_test.cpp"
)

gtest_discover_tests(MaiaGameEngineUnitTest)
