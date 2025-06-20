#include "TableUtils.hpp"

#include <fstream>
#include <ios>
#include <iostream>

#include "table.hpp"




Table TableUtils::getTable(const string &tablename) {
    ifstream table_schema_file("data/tablecache/table.cache", ios::binary | ios::in);
    if (!table_schema_file) {
        cout << "tableschema not found" << endl;
        throw runtime_error("Schema cache file not found");
    }

    while (table_schema_file.peek() != EOF) {
        char nameBuffer[32] = {0};
        table_schema_file.read(nameBuffer, 32);
        string currentName(nameBuffer);

        if (currentName == tablename) {
            uint64_t tableMemoryLocation;
            table_schema_file.read(reinterpret_cast<char*>(&tableMemoryLocation), sizeof(tableMemoryLocation));
            cout << "Table '" << tablename << "' found at offset: " << tableMemoryLocation << endl;

            ifstream db_details_file("data/main.db", ios::binary | ios::in);
            if (!db_details_file) {
                cout << "main.db not found" << endl;
                throw runtime_error("Main DB file not found");
            }

            // Move to table metadata location and read the offset for actual table schema
            // Move to actual schema data
            db_details_file.seekg(tableMemoryLocation+40);

            // Read column count
            uint32_t columnCount;
            db_details_file.read(reinterpret_cast<char*>(&columnCount), sizeof(uint32_t));

            vector<Column> columns;
            for (uint32_t i = 0; i < columnCount; ++i) {
                char colnameBuffer[32] = {0};
                db_details_file.read(colnameBuffer, 32);
                string colname(colnameBuffer);

                uint8_t typeInt;
                db_details_file.read(reinterpret_cast<char*>(&typeInt), sizeof(uint8_t));
                DataType dtype = static_cast<DataType>(typeInt);

                columns.emplace_back(colname, dtype);
            }

            // Construct and return the table
            Table table(tablename, columns);
            return table;
        } else {
            // Skip memory location (uint64_t) if name doesn't match
            table_schema_file.seekg(sizeof(uint64_t), ios::cur);
        }
    }

    cout << "Table '" << tablename << "' not found in cache." << endl;
    throw runtime_error("Table not found");
}



