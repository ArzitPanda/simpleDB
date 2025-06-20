//
// Created by minister on 14/6/25.
//


#include "table_manager.hpp"
#include "utils/TableUtils.hpp"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
template<typename T>
bool compareNumeric(T lhs, T rhs, CONDITION_TYPE type) {
    switch (type) {
        case EQUAL: return lhs == rhs;
        case NOT_EQUAL: return lhs != rhs;
        case GREATER_THAN: return lhs > rhs;
        case LESS_THAN: return lhs < rhs;
        case GREATER_THAN_EQUAL: return lhs >= rhs;
        case LESS_THAN_EQUAL: return lhs <= rhs;
        default: return false;
    }
}

bool compareString(const string& lhs, const string& rhs, CONDITION_TYPE type) {
    switch (type) {
        case EQUAL: return lhs == rhs;
        case NOT_EQUAL: return lhs != rhs;
        case GREATER_THAN: return lhs > rhs;
        case LESS_THAN: return lhs < rhs;
        case GREATER_THAN_EQUAL: return lhs >= rhs;
        case LESS_THAN_EQUAL: return lhs <= rhs;
        default: return false;
    }
}

bool compareBool(bool lhs, bool rhs, CONDITION_TYPE type) {
    switch (type) {
        case EQUAL: return lhs == rhs;
        case NOT_EQUAL: return lhs != rhs;
        default: return false; // Only EQUAL and NOT_EQUAL make sense for bool
    }
}


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

void TableManager::printRow(string &table, ConditionGroup conditions) {
    ifstream file("data/table/" + table + ".table", ios::binary | ios::in);
    if (!file) {
        cout << "❌ Table not found: " << table << endl;
        return;
    }

    uint32_t rowCount = 0;
    file.read(reinterpret_cast<char*>(&rowCount), sizeof(uint32_t));
    Table fetched_table = TableUtils::getTable(table);

    size_t columnCount = fetched_table.columns.size();
    cout << "📋 Printing filtered rows from table: " << table << "\n";

    for (uint32_t i = 0; i < rowCount; ++i) {
        vector<string> rowValues;

        // Read row
        for (size_t j = 0; j < columnCount; ++j) {
            char buffer[64] = {0};
            file.read(buffer, 64);
            rowValues.emplace_back(buffer);
        }

        // Evaluate row against condition group
        vector<bool> conditionResults;
        for (const auto& cond : conditions.conditions) {
            auto it = find_if(fetched_table.columns.begin(), fetched_table.columns.end(),
                              [&](const Column& col) { return col.name == cond.columnName; });

            if (it == fetched_table.columns.end()) {
                cerr << "⚠️ Column not found: " << cond.columnName << endl;
                conditionResults.push_back(false);
                continue;
            }

            size_t colIndex = distance(fetched_table.columns.begin(), it);
            const string& cell = rowValues[colIndex];
            const string& value = cond.value;

            bool match = false;
            const Column& column = fetched_table.columns[colIndex];

            try {
                switch (column.type) {
                    case DataType::INT: {
                        int lhs = stoi(cell);
                        int rhs = stoi(value);
                        match = compareNumeric(lhs, rhs, cond.type);
                        break;
                    }
                    case DataType::FLOAT: {
                        float lhs = stof(cell);
                        float rhs = stof(value);
                        match = compareNumeric(lhs, rhs, cond.type);
                        break;
                    }
                    case DataType::DOUBLE: {
                        double lhs = stod(cell);
                        double rhs = stod(value);
                        match = compareNumeric(lhs, rhs, cond.type);
                        break;
                    }
                    case DataType::CHAR:
                    case DataType::TEXT: {
                        match = compareString(cell, value, cond.type);
                        break;
                    }
                    case DataType::BOOL: {
                        bool lhs = (cell == "true" || cell == "1");
                        bool rhs = (value == "true" || value == "1");
                        match = compareBool(lhs, rhs, cond.type);
                        break;
                    }
                    default:
                        match = false;
                }
            } catch (const std::exception& e) {
                std::cerr << "⚠️ Failed to compare cell '" << cell << "' with value '" << value << "': " << e.what() << "\n";
                match = false;
            }


            conditionResults.push_back(match);
        }

        // Evaluate combined condition (with AND/OR)
        bool rowMatches = conditionResults.empty() ? true : conditionResults[0];

        for (size_t k = 0; k < conditions.operations.size(); ++k) {
            if (conditions.operations[k] == AND) {
                rowMatches = rowMatches && conditionResults[k + 1];
            } else if (conditions.operations[k] == OR) {
                rowMatches = rowMatches || conditionResults[k + 1];
            }
        }

        // If matched, print row
        if (rowMatches) {
            cout << "✅ Row " << (i + 1) << ": ";
            for (size_t j = 0; j < rowValues.size(); ++j) {
                cout << rowValues[j];
                if (j < rowValues.size() - 1) cout << " | ";
            }
            cout << "\n";
        }
    }
}









