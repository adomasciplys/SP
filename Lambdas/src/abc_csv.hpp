#ifndef ABC_CSV_HPP
#define ABC_CSV_HPP

#include "abc.hpp"

#include <vector>
#include <iosfwd>  // forward declarations of I/O streams

/// Reads the CSV table into a container of ABC
std::vector<ABC> read_abc(std::istream&);

#endif  // ABC_CSV_HPP
