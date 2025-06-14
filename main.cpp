#include <iostream>

#include "table_manager.hpp"
#include "include/db_manager.hpp"


// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    // TIP Press <shortcut actionId="RenameElement"/> when your caret is at the <b>lang</b> variable name to see how CLion can help you rename it.
    DBManager db;
    std::vector<Column> columns = {
        {"id", DataType::INT},
        {"name", DataType::TEXT},
        {"is_admin", DataType::BOOL}
    };
    db.initDatabase("data/main.db");
    Table usersTable = db.addTable("data/main.db","users", columns);
    TableManager tableManager;
    std::vector<std::string> values = {"1", "Alice", "alice@example.com"};
    std::vector<std::string> values2 = {"2", "Alice", "alice@example.com"};
    tableManager.insertRow("users",values);
    tableManager.insertRow("users",values2);
    tableManager.printRow(usersTable);


    // db.printTableMetaData("data/main.db");
    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}