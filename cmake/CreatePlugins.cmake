function(add_plugin)
  cmake_parse_arguments(PLUGIN "EXCLUDE_FROM_ALL" "TARGET;BASE;VERSION" "SOURCES;PUBLIC_HEADERS;CHILDS" ${ARGN})

  if(NOT PLUGIN_TARGET)
    message(FATAL_ERROR "The target name for the plugin to create must be given.")
  endif()

  if(NOT PLUGIN_CHILDS)
    message(FATAL_ERROR "The name of the child(s) class must be given.")
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
    get_property(PLUGINS_INCLUDE TARGET FlakedTuna::FlakedTuna PROPERTY INCLUDE_DIRECTORIES)
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
  foreach(CHILD ${PLUGIN_CHILDS})
    list(
      APPEND
      PLUGIN_REGISTERS
      "static_assert(std::is_base_of<${PLUGIN_BASE},${CHILD}>::value, \"ERROR: FLAKED_TUNA_PLUGIN: Registered ${CHILD} must be of base type ${PLUGIN_BASE}.\");\nstatic_assert((std::is_default_constructible<${CHILD}>::value), \"ERROR: FLAKED_TUNA_PLUGIN: ${CHILD} type is not default constructable.\");\npr->RegisterPlugin<${CHILD},${PLUGIN_BASE}>()"
      )
  endforeach()
  string(REPLACE ";" "\;\n" PLUGIN_REGISTERS "${PLUGIN_REGISTERS}")
  list(APPEND PLUGIN_REGISTERS "\;")

  string(CONFIGURE
        "#include \"PluginRegistry.hpp\"
        \n
        @PLUGIN_INCLUDES@
        \n
        FlakedTuna::PluginRegistry* pr{nullptr}\;
        \n
        extern \"C\" @PLUGIN_EXPORT@ FlakedTuna::PluginRegistry* GetPluginRegistry()
        {
          pr = new FlakedTuna::PluginRegistry()\;\n
          @PLUGIN_REGISTERS@ \n
          return pr\;
        }
        \n
        extern \"C\" @PLUGIN_EXPORT@ void ClosePluginRegistry()
        {
          if(pr != nullptr) { delete pr\; }
        }
        \n
        extern \"C\" @PLUGIN_EXPORT@ int GetPluginVersion(){return @PLUGIN_VERSION@\;}
        "
        FILE_CONTENT @ONLY)
  file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/generated/plugins/Plugin_${PLUGIN_TARGET}.cpp"  ${FILE_CONTENT})
  target_sources(${PLUGIN_TARGET} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated/plugins/Plugin_${PLUGIN_TARGET}.cpp")
  target_include_directories(${PLUGIN_TARGET} PRIVATE ${PLUGINS_INCLUDE})
endfunction()
