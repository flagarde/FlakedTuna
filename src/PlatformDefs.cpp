#include <algorithm>
#include "PlatformDefs.hpp"

namespace FlakedTuna
{

#if defined(_WIN32) || defined(WIN32)
/*******************************************************
* Win32 platform specific
*******************************************************/

	std::pair<std::vector<PLUG_HANDLE>, std::vector<std::pair<int, PluginRegistry*>>> GetPluginHandles(std::string directory, std::string extension)
	{
		std::vector<PLUG_HANDLE> plugHandles;
		registryVector regPointers;

		LPWIN32_FIND_DATAA findData = new WIN32_FIND_DATAA;
		BOOL f_ok = TRUE;

		if (directory.size() == 0)
		{
			directory.append(".");
		}

		if (directory.substr(directory.length() - 1, 1) != "\\")
		{
			directory.append("\\");
		}
		std::string search(directory);
		search.append("*");
		search.append(extension);

		HANDLE findHandle = FindFirstFileA(search.c_str(), findData);

		while ((findHandle != INVALID_HANDLE_VALUE) && (f_ok == TRUE))
		{
			std::string plugPath(directory);
			plugPath.append(findData->cFileName);

			PLUG_HANDLE handle = LoadLibraryA(plugPath.c_str());
			if (handle != NULL)
			{
				// Look for plugin registry in file
				RegFuncPtr PluginRegistryAddr = (RegFuncPtr) GetProcAddress(handle, "GetPluginRegistry");

				// Check if valid plugin
				if (PluginRegistryAddr != NULL)
				{
					// Now get the plugin
					PluginRegistry* pluginRegistry = PluginRegistryAddr();

					int pluginVersion = 0;
					VersionFuncPtr PluginFileVersion = (VersionFuncPtr) GetProcAddress(handle, "GetPluginVersion");
					if (PluginFileVersion != NULL)
					{
						pluginVersion = PluginFileVersion();
					}

					// Save library/registry
					plugHandles.push_back(handle);
					regPointers.push_back(std::make_pair(pluginVersion, pluginRegistry));
				}
				else
				{
					FreeLibrary(handle);
				}
			}

			// Grab next file handle
			f_ok = FindNextFileA(findHandle, findData);
		}

		delete findData;
		return std::make_pair(plugHandles, regPointers);
	}

	void ClosePluginHandles(std::vector<PLUG_HANDLE> handles)
	{
		for (auto iter : handles)
		{
			CloseFuncPtr closeRegistry = (CloseFuncPtr) GetProcAddress(iter, "ClosePluginRegistry");
			if (closeRegistry != NULL)
			{
				closeRegistry();
			}
			FreeLibrary(iter);
		}
	}


#else
/*******************************************************
* *NIX platform specific
*******************************************************/

	std::pair<std::vector<PLUG_HANDLE>, std::vector<std::pair<int, PluginRegistry*>>> GetPluginHandles(std::string directory, std::string extension)
	{
		std::vector<PLUG_HANDLE> plugHandles;
		registryVector regPointers;

		DIR* dirHandle;
		struct dirent* fileInfo;

		if (extension.find_last_of(".") == std::string::npos)
		{
			// Force extension to have a leading dot for easier checking later
			extension.insert(0, ".");
		}

		if (directory.size() == 0)
		{
			// If directory is an empty string, look in current directory
			directory.append(".");
		}

		if (directory.substr(directory.length() - 1, 1) != "/")
		{
			directory.append("/");
		}

		dirHandle = opendir(directory.c_str());

		while ((dirHandle != NULL) && (fileInfo = readdir(dirHandle)) != NULL)
		{
			std::string plugPath(directory);
			plugPath.append(fileInfo->d_name);

			size_t startOfExtension = plugPath.find_last_of('.');
			if (startOfExtension == std::string::npos)
			{
				// No extension separator, assume this is not a file we are looking for
				continue;
			}
			std::string fileExt = plugPath.substr(startOfExtension, plugPath.length() - startOfExtension);
			if (fileExt != extension)
			{
				// Wrong file extension, ignore this also
				continue;
			}

			PLUG_HANDLE handle = dlopen(plugPath.c_str(), RTLD_LAZY);
			if (handle != NULL)
			{
				// Clear the error flag
				dlerror();

				// Look for plugin registry in file
				RegFuncPtr PluginRegistryAddr = (RegFuncPtr)dlsym(handle, "GetPluginRegistry");

				// Check if valid plugin; NULL means no error
				char* error = dlerror();
				if (error == NULL)
				{
					// Now get the plugin
					PluginRegistry* pluginRegistry = PluginRegistryAddr();

					// Clear the error flag again
					dlerror();

					int pluginVersion = 0;
					VersionFuncPtr PluginFileVersion = (VersionFuncPtr)dlsym(handle, "GetPluginVersion");

					// Check if a version is specified
					error = dlerror();
					if (error == NULL)
					{
						pluginVersion = PluginFileVersion();
					}

					// Save library/registry
					plugHandles.push_back(handle);
					regPointers.push_back(std::make_pair(pluginVersion, pluginRegistry));
				}
				else
				{
					dlclose(handle);
				}
			}
		}

		if (dirHandle != NULL)
		{
			closedir(dirHandle);
		}

		return std::make_pair(plugHandles, regPointers);
	}

	void ClosePluginHandles(std::vector<PLUG_HANDLE> handles)
	{
		for (auto iter : handles)
		{
			// Clear the error flag
			dlerror();

			CloseFuncPtr closeRegistry = (CloseFuncPtr)dlsym(iter, "ClosePluginRegistry");

			char* error = dlerror();
			if (error == NULL)
			{
				closeRegistry();
			}
			dlclose(iter);
		}
	}

#endif

} // end namespace
