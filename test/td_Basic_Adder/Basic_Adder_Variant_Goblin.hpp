
#ifndef VARIANT_GOBLIN_HPP
#define VARIANT_GOBLIN_HPP

#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include <string>
#include <variant>



#include "atomic_models\Basic_Adder.hpp" // included because it will already have included all the custom types

//This variant holds every type that the ports can output
using Variant_Goblin = std::variant<int>;
#endif;