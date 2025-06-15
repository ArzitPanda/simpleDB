#ifndef  SQL_COMMAND_BASE
#define SQL_COMMAND_BASE

#pragma once
#include <string>

#include "table_manager.hpp"

class SQLCommandBase {
    public:
        virtual void execute(const TableManager& table_manager) = 0;
        virtual ~SQLCommandBase() {};

};
#endif
