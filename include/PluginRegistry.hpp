#pragma once

#include <exception>
#include <functional>
#include <memory>
#include <typeindex>
#include <map>

// TODO: Look at supporting constructors with parameters

namespace FlakedTuna
{
  class PluginRegistry
  {

  private:
    std::multimap<std::type_index,std::function<std::shared_ptr<void>()>> _plugins;

  public:
    template<typename T, typename BaseT> void RegisterPlugin()
    {
      _plugins.emplace(std::type_index(typeid(BaseT)), [this]() { return std::make_shared<T>(); });
    }

    template <class BaseT> std::vector<std::shared_ptr<BaseT>> ResolvePlugin()
    {
      std::vector<std::shared_ptr<BaseT>> return_plugins;
      std::pair<std::multimap<std::type_index, std::function<std::shared_ptr<void>()>>::iterator, std::multimap<std::type_index, std::function<std::shared_ptr<void>()>>::iterator> ret ;
      ret = _plugins.equal_range(std::type_index(typeid(BaseT)));
      for(std::multimap<std::type_index, std::function<std::shared_ptr<void>()>>::iterator it=ret.first; it!=ret.second; ++it)
      {
        return_plugins.push_back( std::static_pointer_cast<BaseT>( (it->second)() ) );
      }
      return return_plugins;
    }
  };
}  // namespace FlakedTuna
