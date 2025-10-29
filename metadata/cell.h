#pragma once
#include <vector>
#include <string>

struct DataCell{
    int length;
    std::vector<unsigned char> bytes; 
};

struct DataRow{
    std::vector<DataCell> cells;
};

struct DataPage{
    int id;
    std::vector<DataRow> rows;
    int nextPage;
};

enum ColumnType {
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