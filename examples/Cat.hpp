#include "Animal.hpp"

class Cat : public Animal
{
public:
  virtual std::string Description() final;
};
