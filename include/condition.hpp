#ifndef  CONDITION_HPP
#define CONDITION_HPP
#include <string>
#include <vector>


enum  CONDITION_TYPE {
    EQUAL = 0,
    GREATER_THAN=1,
    LESS_THAN=2,
    GREATER_THAN_EQUAL=3,
    LESS_THAN_EQUAL=4,
    NOT_EQUAL=5,
};
enum  CONDITION_OPERATION {
    AND=1,
    OR=2,

};


struct Condition {
    std::string columnName;
    std::string tableName;
    CONDITION_TYPE type;
    std::string value;

    Condition(const std::string& columnName, const std::string& tableName, CONDITION_TYPE type, const std::string& value)
        : columnName(columnName), tableName(tableName), type(type), value(value) {}
};

struct ConditionGroup {
    std::vector<Condition> conditions;
    std::vector<CONDITION_OPERATION> operations;

    ConditionGroup(const std::vector<Condition>& conditions, const std::vector<CONDITION_OPERATION>& operations)
        : conditions(conditions), operations(operations) {}
};



#endif