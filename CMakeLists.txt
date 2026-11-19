cmake_minimum_required(VERSION 3.21)
project(Lab9Refactoring LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
  add_compile_options(/utf-8 /W4)
endif()

enable_testing()
find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

add_subdirectory(library)
add_subdirectory(leaderboard)
