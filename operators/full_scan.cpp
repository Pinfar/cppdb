#include "full_scan.h"
namespace DBCPP_Operators{
    FullScanOperator::FullScanOperator(StorageEngine &storageEngine, std::string tableName) : storageEngine(storageEngine),
        currentPage(&StorageEngine::EMPTY_PAGE)
    {
        auto &header = storageEngine.getDatabaseHeaderPage();
        for (auto &table : header.tables)
        {
            if (table.definition.name == tableName)
            {
                int pageId = table.dataPages[0];
                this->currentPage = storageEngine.getDataPage(pageId);
            }
        }
    }

    bool FullScanOperator::Next()
    {
        auto currentSize = currentPage->rows.size();
        if(currentPosition < ((int)currentPage->rows.size() - 1))
        {
            currentPosition++;
            return true;
        }
        if(currentPage->nextPage == -1)
        {
            return false;
        }
        currentPosition = 0;
        currentPage = storageEngine.getDataPage(currentPage->nextPage);
        return true;
    }


    std::unique_ptr<DataRow> FullScanOperator::Current()
    {
        return std::make_unique<DataRow>(currentPage->rows[currentPosition]);
    }
}