#include "PlatformDefs.hpp"

namespace FlakedTuna
{
  std::pair<std::vector<PLUG_HANDLE>, registryVector > GetPluginHandles(const n_fs::path& path,const std::string& extension)
  {
    std::vector<PLUG_HANDLE> plugHandles;
    registryVector           regPointers;
    std::string pextension=extension;
    // Force extension to have a leading dot for easier checking later
    if(pextension.find_last_of(".") == std::string::npos)
    {
      pextension.insert(0, ".");
    }

    for(auto& p: n_fs::recursive_directory_iterator(path))
    {
      if(p.path().extension()==pextension)
      {

        PLUG_HANDLE handle = LoadLibraryA(p.path().c_str());
        if(handle != nullptr)
        {
          // Look for plugin registry in file
          RegFuncPtr PluginRegistryAddr = (RegFuncPtr)GetProcAddress(handle, "GetPluginRegistry");

          // Check if valid plugin
          if(PluginRegistryAddr != nullptr)
          {
            // Now get the plugin
            PluginRegistry* pluginRegistry = PluginRegistryAddr();

            int            pluginVersion{0};
            VersionFuncPtr PluginFileVersion = (VersionFuncPtr)GetProcAddress(handle, "GetPluginVersion");
            if(PluginFileVersion != nullptr) { pluginVersion = PluginFileVersion(); }

            // Save library/registry
            plugHandles.push_back(handle);
            regPointers.push_back(std::make_pair(pluginVersion, pluginRegistry));
          }
          else
          {
            FreeLibrary(handle);
          }
        }
      }
    }
    return std::make_pair(plugHandles, regPointers);
  }

  void ClosePluginHandles(std::vector<PLUG_HANDLE> handles)
  {
    for(auto iter: handles)
    {
      CloseFuncPtr closeRegistry = (CloseFuncPtr)GetProcAddress(iter, "ClosePluginRegistry");
      if(closeRegistry != nullptr) { closeRegistry(); }
      FreeLibrary(iter);
    }
  }

}
