#pragma once
#include "../storage/storage.h"
#include "abstract_iterator.h"
namespace DBCPP_Operators{
    class FullScanOperator : public AbstractDbOperator {
        private:
            StorageEngine& storageEngine;
            std::string tableName;
            DataPage* currentPage;
            int currentPosition = -1;
            TableDefinition metadata;
        public:
            FullScanOperator(StorageEngine& storageEngine, std::string tableName);
            FullScanOperator(FullScanOperator&) = delete;
            virtual bool Next() override;
            virtual std::unique_ptr<DataRow> Current() override;
            virtual TableDefinition GetMetadata() override;
    };
}