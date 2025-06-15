#include <fstream>
#include <ios>
#include <iostream>

#include "table.hpp"
#ifndef  TABLEUTILS_H
#define TABLEUTILS_H


class TableUtils {
    public:
    Table getTable(const string& tablename);
};


Table TableUtils::getTable(const string &tablename) {
    ifstream table_schema_file("data/tablecache/tabel.cache",ios::binary | ios::in);
    if (!table_schema_file) {
        cout << "tableschema not found" << endl;
    }
    while (table_schema_file.peek()!=EOF) {
    char nameBuffer[32];
        table_schema_file.read(nameBuffer,32);
        if (nameBuffer==tablename) {

            /// continue after sometime i will do
        }

    }



}


