
#include "sql_parser.hpp"

#include <algorithm>
#include <string>


#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <vector>

#include "condition.hpp"
#include "table_manager.hpp"


CONDITION_TYPE mapOperator(const std::string& op) {
        if (op == "=") return EQUAL;
        if (op == "!=") return NOT_EQUAL;
        if (op == ">") return GREATER_THAN;
        if (op == "<") return LESS_THAN;
        if (op == ">=") return GREATER_THAN_EQUAL;
        if (op == "<=") return LESS_THAN_EQUAL;
        throw std::invalid_argument("Unknown operator");
}


void SQLParser::executeSQL(const std::string* sql) {
   std::string query = *sql;
        ranges::transform(query, query.begin(), ::tolower);

        std::stringstream ss(query);
        std::string token;
        std::vector<std::string> tokens;

        while (ss >> token) {
            tokens.push_back(token);
        }

        if (tokens.size() < 4 || tokens[0] != "select") {
            std::cerr << "❌ Invalid or unsupported SQL format.\n";
            return;
        }

        std::vector<std::string> selectedColumns;
        size_t i = 1;
        while (tokens[i] != "from" && i < tokens.size()) {
            if (tokens[i] != ",") selectedColumns.push_back(tokens[i]);
            i++;
        }

        if (i >= tokens.size() - 1) {
            std::cerr << "❌ Missing table name.\n";
            return;
        }

        std::string tableName = tokens[++i];

        std::vector<Condition> conditions;
        std::vector<CONDITION_OPERATION> operations;
        i++;

        if (i < tokens.size() && tokens[i] == "where") {
            i++;
            while (i + 2 < tokens.size()) {
                std::string column = tokens[i++];
                std::string op = tokens[i++];
                std::string val = tokens[i++];

                try {
                    conditions.emplace_back(column, tableName, mapOperator(op), val);
                } catch (const std::exception& e) {
                    std::cerr << "❌ Operator error: " << e.what() << endl;
                    return;
                }

                if (i < tokens.size()) {
                    if (tokens[i] == "and") {
                        operations.push_back(AND); i++;
                    } else if (tokens[i] == "or") {
                        operations.push_back(OR); i++;
                    } else {
                        break;
                    }
                }
            }
        }

        ConditionGroup condGroup(conditions, operations);
        TableManager manager;
        manager.printRow(tableName, condGroup);
}


