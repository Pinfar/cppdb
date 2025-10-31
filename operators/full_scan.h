#pragma once
#include "../storage/storage.h"
#include "abstract_iterator.h"

class FullScanOperator : public AbstractDbOperator {
    private:
        StorageEngine& storageEngine;
        std::string tableName;
        DataPage& currentPage;
        int currentPosition = -1;
    public:
        FullScanOperator(StorageEngine& storageEngine, std::string tableName);
        FullScanOperator(FullScanOperator&) = delete;
        virtual bool Next();
        virtual DataRow& Current();
};