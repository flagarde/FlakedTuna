#include <iostream>

#include "PluginLoader.hpp"
#include "Animal.hpp"

int main(int argc, char** argv)
{
  FlakedTuna::PluginLoader loader;
  loader.FindPluginsAtDirectory(".");

  std::vector<std::shared_ptr<Animal>> plugins = loader.BuildAndResolvePlugin<Animal>();

  for (auto iter : plugins)
  {
    std::cout<<iter->Description()<<std::endl;
  }
  return 1;
}
