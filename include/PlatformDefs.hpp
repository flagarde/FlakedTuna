#pragma once

#include "PluginRegistry.hpp"

#include <string>
#include <vector>

#ifdef LEGACY_CXX
  #include <experimental/filesystem>
  namespace n_fs = ::std::experimental::filesystem;
#else
  #include <filesystem>
  namespace n_fs = ::std::filesystem;
#endif

namespace FlakedTuna
{

  #if defined(_WIN32) || defined(WIN32)
  #define WIN32_LEAN_AND_MEAN
  #include <Windows.h>
  using PLUG_HANDLE = HMODULE;
  #else
  using PLUG_HANDLE = void*;
  #endif

  using RegFuncPtr = PluginRegistry* (*)();
  using CloseFuncPtr = void (*)();
  using VersionFuncPtr = int (*)();


  /*******************************************************
   * Non-platform specific code
   *******************************************************/
  using registryVector = std::vector<std::pair<int, PluginRegistry*>>;

  std::pair<std::vector<PLUG_HANDLE>, registryVector > GetPluginHandles(const n_fs::path& path,const std::string& extension);
  void ClosePluginHandles(std::vector<PLUG_HANDLE> handles);

}
