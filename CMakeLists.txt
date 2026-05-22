cmake_minimum_required(VERSION 3.10...3.17.2)

project(FlakedTuna VERSION "1.1" DESCRIPTION "A lightweight, cross-platform C++17 Plugin Framework" HOMEPAGE_URL "https://github.com/flagarde/FlakedTuna" LANGUAGES CXX)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${PROJECT_SOURCE_DIR}/cmake")

include(CreatePlugins)

option(BUILD_TESTS "Compile the tests" ON)
option(BUILD_EXAMPLES "Compile the examples" ON)

add_subdirectory(src)

if(BUILD_TESTS)
  include(CTest)
  add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
