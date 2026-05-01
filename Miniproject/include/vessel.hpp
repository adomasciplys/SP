#ifndef MINIPROJECT_VESSEL_HPP
#define MINIPROJECT_VESSEL_HPP

#include <string>
#include <map>

struct Vessel
{
    Vessel(std::string);
    [[nodiscard]] const std::map<std::string, int>& environment() const;
private:
    std::string _name;
    std::map<std::string, int> _env;
};

#endif // MINIPROJECT_VESSEL_HPP
