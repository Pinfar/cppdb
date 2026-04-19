#include "cell.h"

int TableDefinition::getColumnIdx(std::string columnName)
{
    for(int i=0; i<columns.size();i++)
    {
        if(columns[i].name == columnName)
        {
            return i;
        }
    }
    return -1;
}