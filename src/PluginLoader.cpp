#include "PluginLoader.hpp"

#include <tuple>

namespace FlakedTuna
{

  #if defined(_WIN32) || defined(WIN32)
  const std::string PluginLoader::m_DefaultExtension=".dll";
  #else
  const std::string PluginLoader::m_DefaultExtension=".so";
  #endif

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
  bool PluginLoader::FindPluginsAtDirectory(const n_fs::path& path,const std::string& extension)
  {
    std::vector<PLUG_HANDLE> newLibs;
    registryVector           newRegs;

    std::tie(newLibs, newRegs) = GetPluginHandles(path, extension);

    if(newLibs.size() == 0)  // newLibs and newRegs are same size, so only check one
    { return false; }
    _libraries.insert(_libraries.end(), newLibs.begin(), newLibs.end());
    _registries.insert(_registries.end(), newRegs.begin(), newRegs.end());
    return true;
  }
}  // namespace FlakedTuna
