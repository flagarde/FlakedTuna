set(INCLUDES "${CMAKE_SOURCE_DIR}/include")

set(PUBLIC_INCLUDE_INSTALL_DIR "include")

add_library(FlakedTuna "PlatformDefs.cpp" "PluginLoader.cpp")
target_include_directories(FlakedTuna PUBLIC $<BUILD_INTERFACE:${INCLUDES}> PUBLIC $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
set_target_properties(FlakedTuna PROPERTIES PUBLIC_HEADER "${INCLUDES}/PluginLoader.hpp;${INCLUDES}/PluginRegistry.hpp;${INCLUDES}/PlatformDefs.hpp")
target_link_libraries(FlakedTuna PRIVATE ${CMAKE_DL_LIBS})
set_property(TARGET FlakedTuna PROPERTY POSITION_INDEPENDENT_CODE ON)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.4)
    message(FATAL_ERROR "You are on an extremely old version of GCC. Please update your compiler to at least GCC 5.4, preferably latest")
  elseif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8.0)
    message(WARNING "Old Verison of GCC detected. Using Legacy C++ support")
    target_link_libraries(${PROJECT_NAME} PRIVATE -lstdc++fs)
    target_compile_definitions(${PROJECT_NAME} PUBLIC LEGACY_CXX)
  endif()
endif()

add_library(FlakedTuna::FlakedTuna ALIAS FlakedTuna)

install(TARGETS FlakedTuna  EXPORT FlakedTuna RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION ${PUBLIC_INCLUDE_INSTALL_DIR})

install(EXPORT FlakedTuna FILE "FlakedTuna.cmake" DESTINATION "lib/cmake/${PROJECT_NAME}" NAMESPACE FlakedTuna::)
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)

configure_package_config_file("${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" INSTALL_DESTINATION "lib/cmake/${PROJECT_NAME}" PATH_VARS PUBLIC_INCLUDE_INSTALL_DIR)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" "${CMAKE_SOURCE_DIR}/cmake/CreatePlugins.cmake" DESTINATION "lib/cmake/${PROJECT_NAME}")


