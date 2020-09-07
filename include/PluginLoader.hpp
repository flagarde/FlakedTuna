#pragma once

#include "PluginRegistry.hpp"
#include "PluginHandler.hpp"

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

  class PluginLoader
  {
  private:
    std::vector<PluginHandler> m_Libraries;
    std::vector<PluginHandler> GetPluginHandles(const n_fs::path& path,const std::string& extension);
  #if defined(_WIN32) || defined(WIN32)
    std::string m_Extension{".dll"};
  #else
    std::string m_Extension{".so"};
  #endif

    void ClosePluginHandles(std::vector<PluginHandler>& handles);
  public:
    PluginLoader()=default;
    ~PluginLoader();
    void ClosePluginLibraries();
    bool FindPluginsAtDirectory(const n_fs::path& path,const std::string& extension="");

    template <class BaseT> std::vector<std::shared_ptr<BaseT>> BuildAndResolvePlugin(const int& version=0)
    {
      std::vector<std::shared_ptr<BaseT>> concretePlugins;

      for(auto registryIter : m_Libraries)
      {
        if(registryIter.getVersion() < version) continue;

        std::vector<std::shared_ptr<BaseT>> concretePlugin = registryIter.getRegistry()->ResolvePlugin<BaseT>();
        for(std::size_t i=0;i!= concretePlugin.size();++i)
        {
          if (concretePlugin[i].get() != nullptr) concretePlugins.push_back(concretePlugin[i]);
        }
      }
      return concretePlugins;
    }
  };

}
