#pragma once
#include <vector>
#include <string>
#include <variant>
#include <memory>

struct DataCell{
    //this is not the most performant way of doing this, but I am still learning,
    //I read reddit post that says it is 70% slower than raw unions!
    //So it might be a hotspot in the future, we are going to see!
    std::variant<std::string, int> value; 
};

struct DataRow{
    std::vector<DataCell> cells;
};

struct DataPage{
    int id;
    std::vector<DataRow> rows;
    int nextPage;
};

enum class ColumnType {
    Int, String
};

struct Column
{
    std::string name;
    ColumnType type;
};

struct TableDefinition{
    std::string name;
    std::vector<Column> columns;
};

struct TableHeader
{
    int tableId;
    TableDefinition definition;
    std::vector<int> dataPages;
};

struct DataBaseHeaderPage{
    std::vector<TableHeader> tables;
};