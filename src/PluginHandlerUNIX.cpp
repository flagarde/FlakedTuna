#include "PluginHandler.hpp"

#include "dlfcn.h"

namespace FlakedTuna
{

bool PluginHandler::open(const std::string& name)
{
  if(!m_Opened)
  {
    m_Handle = dlopen(name.c_str(), RTLD_LAZY);
    if(m_Handle != nullptr) m_Opened=true;
    else m_Opened=false;
  }
  return m_Opened;
}

bool PluginHandler::close()
{
  if(m_Opened)
  {
    // Clear the error flag
    dlerror();
    CloseFuncPtr closeRegistry = (CloseFuncPtr)dlsym(m_Handle, "ClosePluginRegistry");
    char* error{dlerror()};
    if(error == nullptr)
    {
      closeRegistry();
    }
    dlclose(m_Handle);
    m_Opened=false;
  }
  return m_Opened;
}

bool PluginHandler::load()
{
  if(m_Handle != nullptr)
  {
    // Clear the error flag
    dlerror();

    // Look for plugin registry in file
    RegFuncPtr PluginRegistryAddr = (RegFuncPtr)dlsym(m_Handle, "GetPluginRegistry");

    // Check if valid plugin; nullptr means no error
    char* error{dlerror()};
    if (error == nullptr)
    {
      // Now get the plugin
      m_Registry = PluginRegistryAddr();

      // Clear the error flag again
      dlerror();

      VersionFuncPtr PluginFileVersion = (VersionFuncPtr)dlsym(m_Handle, "GetPluginVersion");

      // Check if a version is specified
      error = dlerror();
      if (error == nullptr)
      {
        m_Version = PluginFileVersion();
        return true;
      }
      else return false;
    }
    else
    {
      dlclose(m_Handle);
      return false;
    }
  }
  else return false;
}

};
