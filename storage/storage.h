#pragma once
#include "../metadata/cell.h"
#include <memory>

class StorageEngine{
    private:
        DataBaseHeaderPage headerPage;
        std::vector<DataPage> dataPageCache;
    public:
        StorageEngine();
        StorageEngine(TableHeader header, std::vector<DataPage> cache);
        const DataBaseHeaderPage& getDatabaseHeaderPage();
        const DataPage& getDataPage(int id);
        const static DataPage EMPTY_PAGE;
};