cmake_minimum_required(VERSION 3.1)

project(logo_recoginition LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package( OpenCV REQUIRED)

set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/catch2)
add_library(Catch2::Catch IMPORTED INTERFACE)
set_property(TARGET Catch2::Catch PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${CATCH_INCLUDE_DIR}")

add_subdirectory(src)
add_subdirectory(app)
add_subdirectory(tests)
