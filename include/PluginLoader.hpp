#pragma once

#include <map>
#include <vector>
#include <string>
#include "PluginRegistry.hpp"
#include "PlatformDefs.hpp"

namespace FlakedTuna
{
	class PluginLoader
	{
	private:
		std::vector<PLUG_HANDLE> _libraries;
		registryVector _registries;

		void ClosePluginLibraries();

	public:
		~PluginLoader();

		bool FindPluginsAtDirectory(std::string additionalDir, std::string extension);

		template <class BaseT>
		std::vector<std::shared_ptr<BaseT>> BuildAndResolvePlugin( int version = 0 )
		{
			std::vector<std::shared_ptr<BaseT>> concretePlugins;

			for (auto registryIter : _registries)
			{
				if (registryIter.first < version)	continue; // Earlier versions may not be forward compatible, so ignore

				std::shared_ptr<BaseT> concretePlugin = registryIter.second->ResolvePlugin<BaseT>();
				if (concretePlugin.get() != nullptr)
				{
					// It has this base type registered
					concretePlugins.push_back(concretePlugin);
				}
			}

			return concretePlugins;
		}
	};
}
