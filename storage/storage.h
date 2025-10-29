#pragma once
#include "../metadata/cell.h"
#include <memory>

class StorageEngine{
    private:
        DataBaseHeaderPage headerPage;
        std::vector<DataPage> dataPageCache;
    public:
        StorageEngine();
        StorageEngine(StorageEngine&) = delete;
        StorageEngine(TableHeader header, std::vector<DataPage> cache);
        DataBaseHeaderPage& getDatabaseHeaderPage();
        DataPage& getDataPage(int id);
        static DataPage EMPTY_PAGE;
};