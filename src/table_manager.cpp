//
// Created by minister on 14/6/25.
//


#include "table_manager.hpp"

#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>


bool TableManager::insertRow(const string &tablename, vector<string> &values) {
    fstream file("data/table/"+tablename+".table",ios::in | ios::binary | ios::out);
    if (!file) {
        std::cerr << "<UNK> Failed to open table file: " << tablename << "\n";
        return false;
    }
    uint32_t rowCount;
    if (!file.read(reinterpret_cast<char*>(&rowCount), sizeof(uint32_t))) {
        // File might be empty or row count not written yet
        rowCount = 0;
        std::cout << "⚠️  Row count not found. Initializing to 0.\n";

    }
    file.seekp(0,ios::end);
    for (string &value : values) {
        char buffer[64] ={0};
        strncpy(buffer, value.c_str(), 63);
        file.write(buffer, 64);
    }
    rowCount += 1;
    file.seekp(0);
    file.write(reinterpret_cast<char*>(&rowCount), sizeof(uint32_t));
    cout << "✅ Row inserted into " << tablename << "\n";
    cout << rowCount << " rows inserted.\n";
    return true;

}

void TableManager::printRow(Table &table) {
    ifstream file("data/table/"+table.name+".table", ios::binary | ios::in);
    uint32_t rowCount = 0;
    if (!file) {
        cout << "table not found" << endl;
        return;
    }

    file.read(reinterpret_cast<char*>(&rowCount), sizeof(uint32_t));

    cout << "📊 Total Rows: " << rowCount << "\n";
    for (uint32_t i = 0; i < rowCount; ++i) {
        cout << "🔸 Row " << (i + 1) << ": ";
        for (size_t j = 0; j < table.columns.size(); ++j) {
            char buffer[64] = {0};
            file.read(buffer, 64);
            cout << buffer;
            if (j < table.columns.size() - 1) cout << " | ";
        }
        cout << "\n";
    }


}


