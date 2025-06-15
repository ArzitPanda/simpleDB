#include "db_manager.hpp"

#include <cstring>
#include <filesystem>
using namespace  std;
#include <fstream>
#include <iostream>

bool DBManager::initDatabase(const string &name) {
    std::filesystem::create_directories("data");
    ofstream file(name,ios::binary | ios::trunc);
    if (!file) {
        std::cerr << "Cannot open db file " << name << endl;
        return false;
    }
    //what i understand we need a header name
    const char magic[4] = {'T','I','N','Y'};
    file.write(magic,4);
    //write a version number
    u_int32_t version =1;
    file.write(reinterpret_cast<char*>(&version),sizeof(u_int32_t));

    size_t tablecount =0;
    file.write(reinterpret_cast<char*>(&tablecount),sizeof(u_int32_t));


    cout << "Table size: " << tablecount << endl;
    return true;

}

Table DBManager::addTable(const string &filename, const string &table, const vector<Column> &c) {
    fstream file(filename, ios::binary | ios::in | ios::out);
    if (!file) {
        throw std::runtime_error("Cannot open DB file: " + filename);
    }

    // Read table count
    file.seekg(8);
    uint32_t tableCount = 0;
    file.read(reinterpret_cast<char*>(&tableCount), sizeof(uint32_t));

    size_t entryoffset = 12;
    std::cout << "🔍 Starting metadata section at offset: " << entryoffset << "\n";

    if (tableCount > 0) {
        for (uint32_t i = 0; i < tableCount; i++) {
            std::cout << "\n📄 Processing table #" << (i + 1) << "...\n";
            file.seekg(entryoffset + 32);

            file.read(reinterpret_cast<char*>(&entryoffset), sizeof(uint64_t));
            std::cout << "   🔹 Read table offset: " << entryoffset << "\n";
            std::cout << "   🔹 Seeking to (offset + 40): " << (entryoffset + 40) << "\n";

            uint32_t columncount = 0;
            file.read(reinterpret_cast<char*>(&columncount), sizeof(uint32_t));
            std::cout << "   🔸 Column count: " << columncount << "\n";

            size_t schemaSize = columncount * (32 + sizeof(uint8_t));
            entryoffset += schemaSize + 4;
            std::cout << "   🔁 New entry offset after processing this schema: " << entryoffset << "\n";
        }
    }

    char nameBuffer[32] = {0};
    strncpy(nameBuffer, table.c_str(), 31);
    ofstream fileCache("data/tablecache/table.cache", ios::binary | ios::in| ios::out);
    if (!fileCache) {
        std::cerr << "Cannot open table cache" << endl;
    }
    fileCache.seekp(0,ios::end);
    fileCache.write(nameBuffer,32);
    fileCache.write(reinterpret_cast<char*>(&entryoffset), sizeof(uint64_t));

    file.seekp(entryoffset);
    file.write(nameBuffer, 32);

    uint64_t tableOffset = entryoffset + 32 + 8;
    file.write(reinterpret_cast<char*>(&tableOffset), sizeof(uint64_t));

    // Increment and update table count
    tableCount++;
    file.seekp(8);
    file.write(reinterpret_cast<char*>(&tableCount), sizeof(uint32_t));

    // Write schema
    Table tableGen(table, c);
    writeTableSchema(file, tableGen, tableOffset);

    // Create .table data file
    std::string tableDataFile = "data/table/" + table + ".table";
    std::filesystem::create_directories("data/table");
    ofstream tableFile(tableDataFile, ios::binary | ios::trunc|ios::out|ios::in);
    uint32_t rowCount = 0;
    tableFile.write(reinterpret_cast<char*>(&rowCount), sizeof(uint32_t));
    if (!tableFile) {
        throw std::runtime_error("Failed to create table data file: " + tableDataFile);
    }

    std::cout << "📂 Table data file created: " << tableDataFile << "\n";
    std::cout << "✅ Table '" << table << "' added at offset " << tableOffset << "\n";

    return tableGen;
}


void DBManager::printTableMetaData(const string &filename) {
    ifstream file(filename,ios::binary | ios::in);
    if (!file) {
        std::cerr << "Cannot open db file " << filename << std::endl;
        return;
    }
    char magic[4];
    file.read(magic,4);
    uint32_t version ;
    file.read(reinterpret_cast<char*>(&version),sizeof(uint32_t));
    uint32_t tableCount;
    file.read(reinterpret_cast<char*>(&tableCount),sizeof(uint32_t));
    std::cout << "📄 Magic: " << std::string(magic, 4) << "\n";
    std::cout << "🔢 Version: " << version << "\n";
    std::cout << "📊 Table Count: " << tableCount << "\n";
    // file.seekg(16);

    for (uint32_t i = 0; i < tableCount; i++) {
        char nameBuffer[32]={0};
        file.read(nameBuffer,32);
        string tableName(nameBuffer);

        uint64_t tableOffset;
        file.read(reinterpret_cast<char*>(&tableOffset),sizeof(uint64_t));
        std::cout << "📁 Table " << (i + 1) << ": " << tableName
                 << " | Offset: " << tableOffset << "\n";
        uint32_t columnCount;
        file.read(reinterpret_cast<char*>(&columnCount),sizeof(uint32_t));
        cout << "<UNK> Column Count: " << columnCount << "\n";
        for (uint32_t j = 0; j < columnCount; j++) {
            char columnName[32] = {0};
            file.read(columnName, 32);

            uint8_t columnType;
            file.read(reinterpret_cast<char*>(&columnType), sizeof(uint8_t));

            std::cout << "   🧱 Column " << (j + 1)
                      << ": Name = " << columnName
                      << ", Type = " << static_cast<int>(columnType) << "\n";
        }

        std::cout << "Current file position: " << file.tellg() << "\n";
    }

}

void DBManager::writeTableSchema(fstream &file, Table &table, uint64_t offset) {
    file.seekp(offset);
    uint32_t columnCount = table.columns.size();
    file.write(reinterpret_cast<char*>(&columnCount),sizeof(uint32_t));
    for (const Column &column : table.columns) {
        char colnameBuffer[32]={0};
        strncpy(colnameBuffer,column.name.c_str(),31);
        file.write(colnameBuffer,32);
        uint8_t type = static_cast<uint8_t>(column.type);
        file.write(reinterpret_cast<char*>(&type),sizeof(uint8_t));
    }
    std::cout << "Schema written at offset " << offset << " with " << columnCount << " columns\n";

}



