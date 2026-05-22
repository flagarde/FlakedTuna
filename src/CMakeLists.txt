set(PUBLIC_INCLUDE_INSTALL_DIR "include")

if(WIN32)
  add_library(FlakedTuna "PlatformWindows.cpp" "PluginLoader.cpp")
elseif(UNIX)
  add_library(FlakedTuna "PlatformUNIX.cpp" "PluginLoader.cpp")
else()
  message(FATAL_ERROR "FlakedTuna is only available for UNIX and Windows systems")
endif()
target_include_directories(FlakedTuna PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${INCLUDE_OUTPUT_DIR}>)
set_target_properties(FlakedTuna PROPERTIES PUBLIC_HEADER "${PROJECT_SOURCE_DIR}/include/flakedthuna/PluginLoader.hpp;${PROJECT_SOURCE_DIR}/include/flakedthuna/PluginRegistry.hpp;${PROJECT_SOURCE_DIR}/include/flakedthuna/PlatformDefs.hpp")
target_link_libraries(FlakedTuna PRIVATE ${CMAKE_DL_LIBS})
set_property(TARGET FlakedTuna PROPERTY POSITION_INDEPENDENT_CODE ON)
target_compile_features(FlakedTuna PUBLIC cxx_std_17)


add_library(FlakedTuna::FlakedTuna ALIAS FlakedTuna)

install(TARGETS FlakedTuna  EXPORT FlakedTuna RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBRARY_OUTPUT_DIR} ARCHIVE DESTINATION ${ARCHIVE_OUTPUT_DIR} PUBLIC_HEADER DESTINATION ${PUBLIC_INCLUDE_INSTALL_DIR})

install(EXPORT FlakedTuna FILE "FlakedTuna.cmake" DESTINATION "lib/cmake/${PROJECT_NAME}" NAMESPACE FlakedTuna::)
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)

configure_package_config_file("${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" INSTALL_DESTINATION "lib/cmake/${PROJECT_NAME}" PATH_VARS PUBLIC_INCLUDE_INSTALL_DIR)
install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" "${PROJECT_SOURCE_DIR}/cmake/CreatePlugins.cmake" DESTINATION "lib/cmake/${PROJECT_NAME}")
