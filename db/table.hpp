#ifndef TABLE_HPP
#define TABLE_HPP
#include <cstdint>
#include <string>
#include <variant>
#include <vector>
using namespace std;



enum  class DataType : uint8_t {
    INT =1,
    FLOAT = 2,
    DOUBLE = 3,
    CHAR = 4,
    TEXT = 5,
    BOOL =6,
};

struct  Column {
    string name;
    DataType type;
    Column(const string& n, DataType t) : name(n), type(t) {}
};

struct  Value {
    DataType type;
    variant<int, float, double,char,string,bool> value;
    Value(int val) : type(DataType::INT), value(val) {}
    Value(float val) : type(DataType::FLOAT), value(val) {}
    Value(double val) : type(DataType::DOUBLE), value(val) {}
    Value(char val) : type(DataType::CHAR), value(val) {}
    Value(string val) : type(DataType::TEXT), value(val) {}
    Value(bool val) : type(DataType::BOOL), value(val) {}
};

using  Row = vector<Value>;

struct  Table {
    vector<Column> columns;
    vector<Row> rows;
    string name;

    Table(const string& n, const vector<Column>& c) : columns(c),name(n) {}

};

Table createTable(const string& n, const vector<Column>& c);
void insert(Table &table, const vector<string>& c);
void printTable(const Table &table);
vector<Column> getColumns(Table &table);

#endif
