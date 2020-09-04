cmake_minimum_required(VERSION 3.10...3.17.2)

project(FlakedTuna VERSION "0.9.5" DESCRIPTION "A lightweight, cross-platform C++17 Plugin Framework" HOMEPAGE_URL "https://github.com/flagarde/FlakedTuna" LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(BUILD_TESTS "Compile the tests" ON)
option(BUILD_EXAMPLES "Compile the examples" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)

if(BUILD_TESTS)
  include(CTest)
  add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
