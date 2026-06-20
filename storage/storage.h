#pragma once
#include "../metadata/cell.h"
#include <memory>

class StorageEngine
{
  private:
    DataBaseHeaderPage headerPage;
    std::vector<std::unique_ptr<DataPage>> dataPageCache;
    int nextPageId = 2;

  public:
    StorageEngine(StorageEngine &) = delete;
    StorageEngine(DataBaseHeaderPage header, std::vector<std::unique_ptr<DataPage>> cache)
        : headerPage(header), dataPageCache(std::move(cache))
    {
    }
    DataBaseHeaderPage &getDatabaseHeaderPage();
    DataPage *getDataPage(int id);
    int createNewDataPage();

    static DataPage EMPTY_PAGE;
};