#include "PluginLoader.hpp"

namespace FlakedTuna
{
  PluginLoader::~PluginLoader()
  {
    ClosePluginLibraries();
  }

  void PluginLoader::ClosePluginLibraries()
  {
    ClosePluginHandles(m_Libraries);
    m_Libraries.clear();
  }

  // Return: true  - Success
  //         false - Invalid directory path
  bool PluginLoader::FindPluginsAtDirectory(const n_fs::path& path,const std::string& extension)
  {

    std::vector<PluginHandler> newLibs = GetPluginHandles(path, extension);

    if(newLibs.size() == 0) // newLibs and newRegs are same size, so only check one
    {
      return false;
    }
    m_Libraries.insert(m_Libraries.end(), newLibs.begin(), newLibs.end());
    return true;
  }

  void PluginLoader::ClosePluginHandles(std::vector<PluginHandler>& handles)
  {
    for(auto handle : handles)
    {
      handle.close();
    }
  }

  std::vector<PluginHandler> PluginLoader::GetPluginHandles(const n_fs::path& path,const std::string& extension)
  {
    std::vector<PluginHandler> plugHandles;
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
        PluginHandler handler;
        if(handler.open(p.path().string()))
        {
          handler.load();
          plugHandles.push_back(handler);
        }
      }
    }
    return plugHandles;
  }

}
