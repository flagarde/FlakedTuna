#pragma once

#include <functional>
#include <map>
#include <memory>
#include <typeindex>

// TODO: Look at supporting constructors with parameters

namespace FlakedTuna
{

class PluginRegistry
{
private:
  std::multimap<std::type_index, std::function<std::shared_ptr<void>()>> _plugins;

public:
  template<class T, class BaseT> void RegisterPlugin()
  {
    _plugins.emplace(std::type_index(typeid(BaseT)), [this]() { return std::make_shared<T>(); });
  }

  template <class BaseT> std::vector<std::shared_ptr<BaseT>> ResolvePlugins()
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

}


/*
#if defined(WIN32) || defined(_WIN32)

#define FT_EXPORT	__declspec(dllexport)

#else

#define FT_EXPORT	__attribute__((visibility("default")))

#endif

// Checks
#define FLAKED_TUNA_DERRIVED( concrete, base )		static_assert(std::is_base_of< base , concrete>::value, "ERROR: FLAKED_TUNA_PLUGIN: Registered concrete type must be of base type.");
#define FLAKED_TUNA_DEFAULT_CTOR( concrete )		static_assert((std::is_default_constructible<concrete>::value), "ERROR: FLAKED_TUNA_PLUGIN: Concrete type is not default constructable.");

// Concatenation of checks
#define FLAKED_TUNA_CHECKS( concrete, base )		FLAKED_TUNA_DERRIVED( concrete, base ) FLAKED_TUNA_DEFAULT_CTOR( concrete )

#define FLAKED_TUNA_EXPORTS_BEGIN					FlakedTuna::PluginRegistry* pr = nullptr; extern "C" FT_EXPORT FlakedTuna::PluginRegistry* GetPluginRegistry() { pr = new FlakedTuna::PluginRegistry();
#define FLAKED_TUNA_PLUGIN( concrete, base )		FLAKED_TUNA_CHECKS( concrete, base ) pr->RegisterPlugin< concrete , base >();
#define FLAKED_TUNA_EXPORTS_END						return pr; } extern "C" FT_EXPORT void ClosePluginRegistry() { if(pr != nullptr) { delete pr; } }

#define FLAKED_TUNA_PLUGIN_VERSION( version )		extern "C" FT_EXPORT int GetPluginVersion() { return version; }*/
