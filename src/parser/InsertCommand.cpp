

#include "SQLCommandBase.hpp"
using namespace  std;

class InsertCommand : public  SQLCommandBase {
    string tableName;
    vector<string> values;

    public:
        InsertCommand(const string& name,const vector<string>& values): tableName(name) ,values(values) {}

    void execute(const TableManager &table_manager) override {
            table_manager.insertRow(tableName,values);
        }

};
