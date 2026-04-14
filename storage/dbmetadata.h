#pragma once
#include "storage.h"
namespace DBCPP::Storage {
    //it is a high level structure that provides access to database metadata
    class DbMetadata {
        private:
            StorageEngine* m_storage;
        public:
            DbMetadata(StorageEngine* storage) : m_storage(storage){}
            TableDefinition* GetTableDefinition(std::string tableName);
    };
}