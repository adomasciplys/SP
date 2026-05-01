#include "vessel.hpp"

#include <memory>
#include <utility>

Vessel::Vessel(std::string name) : _name(std::move(name)) { }

const std::map<std::string, int>& Vessel::environment() const
{
    return _env;
}
