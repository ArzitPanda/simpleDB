// tableutils.hpp
#ifndef TABLEUTILS_HPP
#define TABLEUTILS_HPP

#include <string>
#include "table.hpp"

class TableUtils {
public:
    static Table getTable(const std::string& tablename);
};

#endif
