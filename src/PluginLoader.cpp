#include <tuple>
#include "PluginLoader.hpp"

namespace FlakedTuna
{
  PluginLoader::PluginLoader()
  {

  }

  PluginLoader::~PluginLoader()
  {
    ClosePluginLibraries();
  }

  void PluginLoader::ClosePluginLibraries()
  {
    ClosePluginHandles(_libraries);
    _libraries.clear();
    _registries.clear();
  }

  // Return: true  - Success
  //         false - Invalid directory path
  bool PluginLoader::FindPluginsAtDirectory(std::string additionalDir, std::string extension)
  {
    std::vector<PLUG_HANDLE> newLibs;
    registryVector newRegs;

    std::tie(newLibs, newRegs) = GetPluginHandles(additionalDir, extension);

    if (newLibs.size() == 0) // newLibs and newRegs are same size, so only check one
    {
      return false;
    }
    _libraries.insert(_libraries.end(), newLibs.begin(), newLibs.end());
    _registries.insert(_registries.end(), newRegs.begin(), newRegs.end());
    return true;
  }
}
