add_executable(AnalyserTest AnalyserTest.cpp)
target_link_libraries(AnalyserTest PRIVATE BilliardHelper)
add_test(NAME TEST_Analyser COMMAND Analyser WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})