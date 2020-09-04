#pragma once

#include "PlatformDefs.hpp"
#include "PluginRegistry.hpp"

#include <map>
#include <string>
#include <vector>

namespace FlakedTuna
{
  class PluginLoader
  {
  private:
    std::vector<PLUG_HANDLE> _libraries;
    registryVector           _registries;

    void ClosePluginLibraries();

    std::string suffix();
    std::string m_Suffix{""};

  public:
    PluginLoader();
    ~PluginLoader();

    bool FindPluginsAtDirectory(std::string additionalDir, std::string extension = "");

    template<class BaseT> std::vector<std::shared_ptr<BaseT>> BuildAndResolvePlugin(const int& version = 0)
    {
      std::vector<std::shared_ptr<BaseT>> concretePlugins;

      for(auto registryIter: _registries)
      {
        if(registryIter.first < version) continue;  // Earlier versions may not be forward compatible, so ignore

        std::vector<std::shared_ptr<BaseT>> concretePlugin = registryIter.second->ResolvePlugin<BaseT>();
        for(size_t i = 0; i != concretePlugin.size(); ++i)
        {
          if(concretePlugin[i].get() != nullptr)
          {
            // It has this base type registered
            concretePlugins.push_back(concretePlugin[i]);
          }
        }
      }

      return concretePlugins;
    }
  };
}  // namespace FlakedTuna
