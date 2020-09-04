#pragma once

#include <string>
#include <vector>
#include <tuple>
#include "PluginRegistry.hpp"

namespace FlakedTuna
{

/*******************************************************
 * Win32 platform specific
 *******************************************************/
#if defined(_WIN32) || defined(WIN32)

#include <Windows.h>

	typedef HMODULE	PLUG_HANDLE;

/*******************************************************
* *NIX platform specific
*******************************************************/
#else

#include <dlfcn.h>
#include <dirent.h>

	typedef void*	PLUG_HANDLE;

#endif

/*******************************************************
* Non-platform specific code
*******************************************************/
	typedef std::vector < std::pair<int, PluginRegistry*>> registryVector;

	typedef PluginRegistry* (*RegFuncPtr)();
	typedef void (*CloseFuncPtr)();
	typedef int (*VersionFuncPtr)();

	std::pair<std::vector<PLUG_HANDLE>, std::vector<std::pair<int, PluginRegistry*>>> GetPluginHandles(std::string directory, std::string extension);

	void ClosePluginHandles(std::vector<PLUG_HANDLE> handles);

}
