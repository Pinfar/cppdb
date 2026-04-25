#pragma once
#include <vector>
#include <string>
#include <variant>
#include <any>
#include <memory>


enum class ColumnType {
    Unknown, Int, String, Boolean
};

struct DataCell{
    //this is not the most performant way of doing this, but I am still learning,
    //I read reddit post that says it is 70% slower than raw unions!
    //So it might be a hotspot in the future, we are going to see!
    std::any value; 
    ColumnType type;
    DataCell(std::string str){
        value = str;
        type = ColumnType::String;
    }
    DataCell(int val){
        value = val;
        type = ColumnType::Int;
    }
    DataCell(bool val){
        value = val;
        type = ColumnType::Boolean;
    }
};

struct DataRow{
    std::vector<DataCell> cells;
};

struct DataPage{
    int id;
    std::vector<DataRow> rows;
    int nextPage;
};

struct Column
{
    std::string name;
    ColumnType type;
};

struct TableDefinition{
    std::string name;
    std::vector<Column> columns;
    int getColumnIdx(std::string columnName);
    ColumnType getColumnType(std::string columnName);
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