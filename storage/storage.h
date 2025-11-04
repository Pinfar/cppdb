#pragma once
#include "../metadata/cell.h"
#include <memory>

class StorageEngine{
    private:
        DataBaseHeaderPage headerPage;
        std::vector<DataPage> dataPageCache;
    public:
        StorageEngine(StorageEngine&) = delete;
        StorageEngine(DataBaseHeaderPage header, std::vector<DataPage> cache): headerPage(header), 
            dataPageCache(std::move(cache)){}
        DataBaseHeaderPage& getDatabaseHeaderPage();
        DataPage* getDataPage(int id);
        static DataPage EMPTY_PAGE;
};