cmake_minimum_required(VERSION 3.15.0)
project(billiard_helper VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED COMPONENTS core imgcodecs)

add_library(BilliardHelper BilliardHelper.cpp ImageAnalyser.cpp)
target_include_directories(BilliardHelper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(BilliardHelper PRIVATE opencv_core opencv_imgcodecs)

add_executable(billiard_helper WIN32 main.cpp Application.cpp)
target_link_libraries(billiard_helper PRIVATE BilliardHelper Winmm)
target_compile_definitions(billiard_helper PRIVATE UNICODE)

enable_testing()

add_subdirectory(test)