#include <iostream>

#include "PluginLoader.hpp"
#include "Animal.hpp"

int main(int argc, char** argv)
{
  FlakedTuna::PluginLoader loader;
#if defined(WIN32) || defined(_WIN32)
  loader.FindPluginsAtDirectory(".", "dll");
#else
  loader.FindPluginsAtDirectory(".", "so");
#endif

  std::vector<std::shared_ptr<Animal>> plugins = loader.BuildAndResolvePlugin<Animal>();

  for (auto iter : plugins)
  {
    std::cout<<iter->Description()<<std::endl;
  }
  return 1;
}
