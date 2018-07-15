enable_testing ()
set (GoogleTest_DIR "${CMAKE_SOURCE_DIR}/../Utilities/CMake")
find_package (GoogleTest REQUIRED CONFIG)

add_subdirectory ("UnitTest")
add_test (MaiaGameEngineTest MaiaGameEngineUnitTest)
