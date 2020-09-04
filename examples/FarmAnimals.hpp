#include "Animal.hpp"

class Cow : public Animal
{
public:
  virtual std::string Description() final;
};

class Duck : public Animal
{
public:
  virtual std::string Description() final;
};

class Chicken : public Animal
{
public:
  virtual std::string Description() final;
};
