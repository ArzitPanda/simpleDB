#include <iostream>
#include <string>
// Make sure this matches your actual header file location
#include "src/parser/sql_parser.hpp"

int main() {
    SQLParser parser;
    std::string input;

    std::cout << "📥 Welcome to MiniSQL CLI. Type your SELECT queries below (type 'exit' to quit):\n";

    while (true) {
        std::cout << "SQL> ";
        std::getline(std::cin, input);

        if (input == "exit") {
            std::cout << "👋 Exiting CLI. Goodbye!\n";
            break;
        }

        if (input.empty()) continue;

        try {
            parser.executeSQL(&input);
        } catch (const std::exception& e) {
            std::cerr << "❌ Error executing SQL: " << e.what() << std::endl;
        }
    }

    return 0;
}
