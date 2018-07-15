project(MaiaGameEngineUnitTest)

add_executable(MaiaGameEngineUnitTest)
add_executable(Maia::GameEngine::UnitTest ALIAS MaiaGameEngineUnitTest)
gtest_discover_tests(MaiaGameEngineUnitTest)

target_compile_features(MaiaGameEngineUnitTest PRIVATE cxx_std_17)
target_compile_options(MaiaGameEngineUnitTest 
	PUBLIC 
		$<$<COMPILE_LANGUAGE:CXX>:/permissive->
		$<$<COMPILE_LANGUAGE:CXX>:/Za>
		$<$<COMPILE_LANGUAGE:CXX>:/Zc:rvalueCast>
)

target_include_directories(MaiaGameEngineUnitTest 
	PRIVATE 
		"${GOOGLE_TEST_INCLUDE_DIRECTORY}"
		"${CMAKE_SOURCE_DIR}/Source"
)

target_sources(MaiaGameEngineUnitTest 
	
	PRIVATE

		main.cpp

		Transforms/Transform_test.cpp

)

target_link_libraries(MaiaGameEngineUnitTest 
	debug ${GTEST_DEBUG_LIBRARY} optimized ${GTEST_RELEASE_LIBRARY}
	debug ${GTEST_MAIN_DEBUG_LIBRARY} optimized ${GTEST_MAIN_RELEASE_LIBRARY}
	debug ${GMOCK_DEBUG_LIBRARY} optimized ${GMOCK_RELEASE_LIBRARY}
	debug ${GMOCK_MAIN_DEBUG_LIBRARY} optimized ${GMOCK_MAIN_RELEASE_LIBRARY}
)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries(MaiaGameEngineUnitTest Maia::GameEngine)
