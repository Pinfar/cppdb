#pragma once
#include "../metadata/cell.h"
#include <memory>

class StorageEngine{
    private:
        std::shared_ptr<DataBaseHeaderPage> headerPage;
        std::vector<std::shared_ptr<DataPage>> dataPageCache;
    public:
        StorageEngine();
        StorageEngine(TableHeader header, std::vector<std::shared_ptr<DataPage>> cache);
        std::shared_ptr<DataBaseHeaderPage> getDatabaseHeaderPage();
        std::shared_ptr<DataPage> getDataPage(int id);
};