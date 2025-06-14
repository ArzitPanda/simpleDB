// include/db_manager.hpp
#ifndef DBMANAGER_HPP
#define DBMANAGER_HPP

#include <string>

#include "../db/table.hpp"
using namespace  std;
class DBManager {
    public:
        static bool initDatabase(const string& name);
        Table addTable(const string &filename, const string &table, const vector<Column> &c);
        void printTableMetaData(const string& filename);
        void writeTableSchema(fstream& file, Table& table,uint64_t offset);
};

#endif
