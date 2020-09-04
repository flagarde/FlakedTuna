#include "Animal.hpp"

class Dog : public Animal
{
public:
  virtual std::string Description() final;
};
