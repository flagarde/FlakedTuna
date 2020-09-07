#pragma once

#include <string>

#if defined(_WIN32) || defined(WIN32)
  #define WIN32_LEAN_AND_MEAN
  #include "windows.h"
  typedef HMODULE PLUG_HANDLE;
#else
  typedef void* PLUG_HANDLE;
#endif

class PluginRegistry;

namespace FlakedTuna
{

  using RegFuncPtr = PluginRegistry* (*)();
  using CloseFuncPtr = void (*)();
  using VersionFuncPtr = int (*)();

  class PluginHandler
  {
  public:
    bool open(const std::string& name);
    bool close();
    bool load();
    int getVersion()
    {
      return m_Version;
    }
    PluginRegistry* getRegistry()
    {
      return m_Registry;
    }
  private:
    // It looks that in linux and windows the handle is void*
    PLUG_HANDLE m_Handle{nullptr};
    bool m_Opened{false};
    int m_Version{0};
    PluginRegistry* m_Registry{nullptr};
  };

}
