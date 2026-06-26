#include "formatter.h"
#include "serialization.h"
#include <vector>

std::string GetSerializedHeader(std::vector<std::string> &columns)
{
    std::string result = "";
    for (int i = 0; i < columns.size(); i++)
    {
        if (i > 0)
            result += ";";
        result += columns[i];
    }
    return result;
}

std::string GetSerializedRow(std::unique_ptr<DataRow> &row)
{
    std::string result = "";
    for (int i = 0; i < row->cells.size(); i++)
    {
        if (i > 0)
            result += ";";
        result += GetStringValue(row->cells[i]);
    }
    return result;
}

std::string GetSerializedResult(std::vector<std::unique_ptr<DataRow>> &result, std::vector<std::string> &columns)
{
    auto serialized = GetSerializedHeader(columns) + "\n";
    for (auto &row : result)
    {
        serialized += GetSerializedRow(row) + "\n";
    }
    return serialized;
}

std::string GetSerializedOpearatorOutput(DBCPP_Operators::AbstractDbOperator &oper, std::vector<std::string> &columns)
{
    auto serialized = GetSerializedHeader(columns) + "\n";
    while (oper.Next())
    {
        auto row = oper.Current();
        serialized += GetSerializedRow(row) + "\n";
    }
    return serialized;
}

std::string GetStringValue(DataCell &cell)
{
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