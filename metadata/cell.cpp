#include "cell.h"
#include <cstddef>

int TableDefinition::getColumnIdx(std::string columnName)
{
    for (size_t i = 0; i < columns.size(); i++)
    {
        if (columns[i].name == columnName)
        {
            return (int)i;
        }
    }
    return -1;
}

ColumnType TableDefinition::getColumnType(std::string columnName)
{
    for (auto &column : columns)
    {
        if (column.name == columnName)
        {
            return column.type;
        }
    }
    return ColumnType::Unknown;
}
