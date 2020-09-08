#include "PlatformDefs.hpp"

#include <dlfcn.h>

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

    // Iterate over the elements inside the directory
    for(auto& p: n_fs::recursive_directory_iterator(path))
    {
      if(p.path().extension()==pextension)
      {
        PLUG_HANDLE handle = dlopen(p.path().string().c_str(), RTLD_LAZY);
          if(handle != nullptr)
          {
            // Clear the error flag
            dlerror();

            // Look for plugin registry in file
            RegFuncPtr PluginRegistryAddr = (RegFuncPtr)dlsym(handle, "GetPluginRegistry");

            // Check if valid plugin; NULL means no error
            char* error = dlerror();
            if(error == nullptr)
            {
              // Now get the plugin
              PluginRegistry* pluginRegistry = PluginRegistryAddr();

              // Clear the error flag again
              dlerror();

              int            pluginVersion{0};
              VersionFuncPtr PluginFileVersion = (VersionFuncPtr)dlsym(handle, "GetPluginVersion");

              // Check if a version is specified
              error = dlerror();
              if(error == nullptr) { pluginVersion = PluginFileVersion(); }

              // Save library/registry
              plugHandles.push_back(handle);
              regPointers.push_back(std::make_pair(pluginVersion, pluginRegistry));
            }
            else
            {
              dlclose(handle);
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
      // Clear the error flag
      dlerror();
      CloseFuncPtr closeRegistry = (CloseFuncPtr)dlsym(iter, "ClosePluginRegistry");
      char* error = dlerror();
      if(error == nullptr) { closeRegistry(); }
      dlclose(iter);
    }
  }

}
