#pragma once

#include <exception>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>

// TODO: Look at supporting constructors with parameters
// TODO: Profile map vs unordered_map

namespace FlakedTuna
{
  class PluginRegistry
  {
  private:
    std::unordered_map<std::type_index, std::vector<std::function<std::shared_ptr<void>()>>> _plugins;

  public:
    template<typename T, typename BaseT, typename... Ts> void RegisterPlugin(Ts... rest)
    {
      // If key already exists, throw a duplicate plugin exception
      std::shared_ptr<T> toto{std::make_shared<T>(rest...)};
      _plugins[std::type_index(typeid(BaseT))].push_back([toto]() { return toto; });
    }

    template<class T> std::vector<std::shared_ptr<T>> ResolvePlugin()
    {
      std::vector<std::shared_ptr<T>> ret = {nullptr};
      if(_plugins.find(std::type_index(typeid(T))) != _plugins.end())
      {
        for(size_t i = 0; i != _plugins[typeid(T)].size(); ++i) ret.push_back(std::static_pointer_cast<T>(_plugins[std::type_index(typeid(T))][i]()));
      }
      return ret;
    }
  };
}  // namespace FlakedTuna
