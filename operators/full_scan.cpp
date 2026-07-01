#include "full_scan.h"
#include <memory>
namespace DBCPP_Operators
{
FullScanOperator::FullScanOperator(StorageEngine *storageEngine, std::string tableName)
    : m_storageEngine(storageEngine), currentPage(&StorageEngine::EMPTY_PAGE)
{
    auto &header = storageEngine->getDatabaseHeaderPage();
    for (auto &table : header.tables)
    {
        if (table.definition.name == tableName)
        {
            metadata = table;
            int pageId = table.dataPages[0];
            this->currentPage = storageEngine->getDataPage(pageId);
        }
    }
}

bool FullScanOperator::Next()
{
    if (currentPosition < ((int)currentPage->rows.size() - 1))
    {
        currentPosition++;
        return true;
    }
    if (currentPage->nextPage == -1)
    {
        return false;
    }
    currentPosition = 0;
    currentPage = m_storageEngine->getDataPage(currentPage->nextPage);
    return true;
}

std::unique_ptr<DataRow> FullScanOperator::Current()
{
    return std::make_unique<DataRow>(currentPage->rows[currentPosition]);
}

void FullScanOperator::Reset()
{
    int pageId = metadata.dataPages[0];
    currentPage = m_storageEngine->getDataPage(pageId);
    currentPosition = -1;
}

auto FullScanOperator::GetOutputSchema() -> QuerySchema
{
    auto copy = metadata.definition.columns;
    return {copy};
}

auto FullScanExecutionPlanNode::Translate(TranslateContext *context) -> DbOperator_Ptr
{
    return std::make_unique<FullScanOperator>(context->storageEngine, m_tableName);
}

} // namespace DBCPP_Operators