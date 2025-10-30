#pragma once
#include "../storage/storage.h"

class FullScanOperator {
    private:
        StorageEngine& storageEngine;
        std::string tableName;
        DataPage& currentPage;
        int currentPosition = -1;
    public:
        FullScanOperator(StorageEngine& storageEngine, std::string tableName);
        FullScanOperator(FullScanOperator&) = delete;
        bool Next();
        DataRow& Current();
};