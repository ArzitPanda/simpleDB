#ifndef TABLE_MANAGER_HPP
#define TABLE_MANAGER_HPP
#include <string>
#include <vector>

#include "condition.hpp"
#include "../db/table.hpp"

using  namespace  std;

class TableManager {
    public:
        static bool insertRow(const string& tablename,vector<string> &values);
        static  void printRow(Table& table);
        static  void printRow(Table& table,ConditionGroup conditions);

};

#endif