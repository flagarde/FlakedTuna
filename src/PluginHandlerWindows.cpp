#include "PluginHandler.hpp"

namespace FlakedTuna
{

bool PluginHandler::open(const std::string& name)
{
  if(!m_Opened)
  {
    m_Handle = LoadLibraryA(name.c_str());
    if(m_Handle != nullptr) m_Opened=true;
    else m_Opened=false;
  }
  return m_Opened;
}

bool PluginHandler::load()
{
  if(m_Handle != nullptr)
  {
    // Look for plugin registry in file
    RegFuncPtr PluginRegistryAddr = (RegFuncPtr) GetProcAddress(m_Handle, "GetPluginRegistry");

    // Check if valid plugin
    if (PluginRegistryAddr != nullptr)
    {
      // Now get the plugin
      m_Registry = PluginRegistryAddr();

      VersionFuncPtr PluginFileVersion = (VersionFuncPtr) GetProcAddress(m_Handle, "GetPluginVersion");
      if(PluginFileVersion != nullptr)
      {
        m_Version = PluginFileVersion();
        return true;
      }
    }
    else
    {
      FreeLibrary(m_Handle);
      return false;
    }
  }
  else return false;
}

bool PluginHandler::close()
{
  CloseFuncPtr closeRegistry = (CloseFuncPtr) GetProcAddress(m_Handle, "ClosePluginRegistry");
  if(closeRegistry != nullptr)
  {
    closeRegistry();
  }
  FreeLibrary(m_Handle);
  return true;
}

};
