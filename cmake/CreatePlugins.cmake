function(add_plugin)
  cmake_parse_arguments(PLUGIN "EXCLUDE_FROM_ALL" "TARGET;VERSION;BASE" "CHILDREN;SOURCES;PUBLIC_HEADERS" ${ARGN})

  if(NOT PLUGIN_TARGET)
    message(FATAL_ERROR "The target name for the plugin to create must be given.")
  endif()

  if(NOT PLUGIN_CHILDREN)
    message(FATAL_ERROR "The name of the child/children class must be given.")
  endif()

  if(NOT PLUGIN_BASE)
    message(FATAL_ERROR "The name of the base class must be given.")
  endif()

  if(NOT PLUGIN_VERSION)
    set(PLUGIN_VERSION "${PROJECT_VERSION_MAJOR}")
  endif()

  if(PLUGIN_EXCLUDE_FROM_ALL)
    add_library(${PLUGIN_TARGET} MODULE EXCLUDE_FROM_ALL ${PLUGIN_SOURCES})
  else()
    add_library(${PLUGIN_TARGET} MODULE ${PLUGIN_SOURCES})
  endif()

  if(TARGET FlakedTuna::FlakedTuna)
    get_target_property(INCLUDE_DIR FlakedTuna::FlakedTuna INCLUDE_DIRECTORIES)
    target_include_directories(${PLUGIN_TARGET} PRIVATE ${INCLUDE_DIR})
  endif()

  # Includes
  if(PLUGIN_PUBLIC_HEADERS)
    list(REMOVE_DUPLICATES PLUGIN_PUBLIC_HEADERS)
    set_target_properties(${PLUGIN_TARGET} PROPERTIES PUBLIC_HEADER "${PLUGIN_PUBLIC_HEADERS}")
    foreach(PUBLIC_HEADER ${PLUGIN_PUBLIC_HEADERS})
      get_filename_component(PUBLIC_HEADER_PATH "${PUBLIC_HEADER}" REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
      list(APPEND PLUGIN_INCLUDES "#include \"${PUBLIC_HEADER_PATH}\"")
      string(REPLACE ";" "\n" PLUGIN_INCLUDES "${PLUGIN_INCLUDES}")
    endforeach()
  else()
    message(FATAL_ERROR "No PUBLIC_HEADERS or given !")
  endif()

  # Export
  if(WIN32)
    set(PLUGIN_EXPORT "__declspec(dllexport)")
  else()
    set(PLUGIN_EXPORT "__attribute__((visibility(\"default\")))")
  endif()

  # REGISTERS
  set(PLUGIN_REGISTERS "")
  foreach(CHILDREN ${PLUGIN_CHILDREN})
    list(
      APPEND
      PLUGIN_REGISTERS
      "static_assert(std::is_base_of<${PLUGIN_BASE},${CHILDREN}>::value, \"ERROR: FLAKED_TUNA_PLUGIN: Registered ${CHILDREN} must be of base type ${PLUGIN_BASE}.\");\nstatic_assert((std::is_default_constructible<${CHILDREN}>::value), \"ERROR: FLAKED_TUNA_PLUGIN: ${CHILDREN} type is not default constructable.\");\npr->RegisterPlugin<${CHILDREN},${PLUGIN_BASE}>()"
      )
  endforeach()
  string(REPLACE ";" "\;\n" PLUGIN_REGISTERS "${PLUGIN_REGISTERS}")
  list(APPEND PLUGIN_REGISTERS "\;")

  string(CONFIGURE
  "#include \"PluginRegistry.hpp\"
   @PLUGIN_INCLUDES@
   FlakedTuna::PluginRegistry* pr{nullptr}\;
   extern \"C\" @PLUGIN_EXPORT@ FlakedTuna::PluginRegistry* GetPluginRegistry()
   {
     pr = new FlakedTuna::PluginRegistry()\;
     @PLUGIN_REGISTERS@
     return pr\;
   }
   \n
   extern \"C\" @PLUGIN_EXPORT@ void ClosePluginRegistry()\n
   {
   if(pr != nullptr) { delete pr\; }
   }
   \n
   extern \"C\" @PLUGIN_EXPORT@ const int GetPluginVersion()
   {
   return @PLUGIN_VERSION@\;
   }"
   FILE_CONTENT @ONLY)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/generated/plugins/Plugin_${PLUGIN_TARGET}.cpp"  ${FILE_CONTENT})
  target_sources(${PLUGIN_TARGET} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated/plugins/Plugin_${PLUGIN_TARGET}.cpp")
endfunction()
