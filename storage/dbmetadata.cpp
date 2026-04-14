#include "dbmetadata.h"

namespace DBCPP::Storage {
    TableDefinition *DbMetadata::GetTableDefinition(std::string tableName)
    {
        auto &header = m_storage->getDatabaseHeaderPage();
        for (auto &table : header.tables)
        {
            if (table.definition.name == tableName)
            {
                return &table.definition;
            }
        }
        return nullptr;
    }
}