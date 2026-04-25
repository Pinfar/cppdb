#include "formatter.h"
#include "serialization.h"

std::string GetSerializedHeader(TableDefinition& table){
    std::string result = "";
    for(int i=0;i<table.columns.size();i++){
        if(i>0)result += ";";
        result += table.columns[i].name;
    }
    return result;
}

std::string GetSerializedRow(std::unique_ptr<DataRow>& row){
    std::string result = "";
    for(int i=0;i<row->cells.size();i++){
        if(i>0)result += ";";
        result += GetStringValue(row->cells[i]);
    }
    return result;
}

std::string GetSerializedResult(std::vector<std::unique_ptr<DataRow>>& result, TableDefinition& table){
    auto serialized = GetSerializedHeader(table) + "\n";
    for(auto& row : result){
        serialized += GetSerializedRow(row) + "\n";
    }
    return serialized;
}


std::string GetSerializedOpearatorOutput(DBCPP_Operators::AbstractDbOperator& oper, TableDefinition& table)
{
    auto serialized = GetSerializedHeader(table) + "\n";
    while(oper.Next()){
        auto row = oper.Current();
        serialized += GetSerializedRow(row) + "\n";
    }
    return serialized;
}

std::string GetStringValue(DataCell& cell){
    switch (cell.type)
    {
    case ColumnType::String:
        return std::any_cast<std::string>(cell.value);
    case ColumnType::Int:
        return std::to_string(std::any_cast<int>(cell.value));    
    default:
        return "";
    }
}