#include "formatter.h"

std::string GetSerializedHeader(TableDefinition& table){
    std::string result = "";
    for(int i=0;i<table.columns.size();i++){
        if(i>0)result += ";";
        result += table.columns[i].name;
    }
    return result;
}

std::string GetSerializedRow(DataRow& row, TableDefinition& table){
    std::string result = "";
    for(int i=0;i<row.cells.size();i++){
        if(i>0)result += ";";
        result += GetStringValue(row.cells[i], table.columns[i]);
    }
    return result;
}

std::string GetStringValue(DataCell& cell, Column& columnData ){
    switch (columnData.type)
    {
    case ColumnType::String:
        return DeserializeAsString(cell);
    case ColumnType::Int:
        return std::to_string(DeserializeAsInt(cell));    
    default:
        return "";
    }
}

std::string DeserializeAsString(DataCell& cell){
    std::string result{
        cell.bytes.begin(), cell.bytes.end()
    };
    return result;
}

int DeserializeAsInt(DataCell& cell){
    auto bytes = cell.bytes;
    int value = int((unsigned char)(bytes[3]) << 24 |
            (unsigned char)(bytes[2]) << 16 |
            (unsigned char)(bytes[1]) << 8 |
            (unsigned char)(bytes[0]));
    return value;
}