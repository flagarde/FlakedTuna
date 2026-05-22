#pragma once

#include "PluginRegistry.hpp"

#include <string>
#include <vector>
#include <filesystem>
#include <utility>
#if defined( _WIN32 ) || defined( WIN32 )
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
#endif


namespace FlakedTuna
{

#if defined( _WIN32 ) || defined( WIN32 )
using PLUG_HANDLE = HMODULE;
#else
using PLUG_HANDLE = void*;
#endif

using RegFuncPtr     = PluginRegistry* (*)();
using CloseFuncPtr   = void ( * )();
using VersionFuncPtr = int ( * )();

/*******************************************************
   * Non-platform specific code
   *******************************************************/
using registryVector = std::vector<std::pair<int, PluginRegistry*>>;

std::pair<std::vector<PLUG_HANDLE>, registryVector> GetPluginHandles( const std::filesystem::path& path, const std::string& extension );
void                                                ClosePluginHandles( std::vector<PLUG_HANDLE> handles );

}  // namespace FlakedTuna
