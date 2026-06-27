#include "dbwriter.h"
#include <stdexcept>

namespace DBCPP_Storage
{

void DBWriter::CreateTable(TableDefinition definition)
{
    auto &headerPage = m_engine.getDatabaseHeaderPage();
    int pageId = m_engine.createNewDataPage();
    headerPage.tables.push_back(TableHeader{nextTableId, definition, {pageId}});

    nextTableId++;
}

void DBWriter::InsertRecord(std::string tableName, DataRow row)
{
    // get first page id
    int firstPageId = -1;
    for (auto &table : m_engine.getDatabaseHeaderPage().tables)
    {
        if (table.definition.name == tableName)
        {
            firstPageId = table.dataPages[0];
        }
    }
    if (firstPageId == -1)
        throw std::logic_error("Invalid table name or table improperly initialized!");

    // find last datapage
    DataPage *page = m_engine.getDataPage(firstPageId);
    while (page->nextPage != -1)
    {
        page = m_engine.getDataPage(page->nextPage);
    }

    // check if it is full
    if (page->rows.size() >= m_options.maxRecordsPerPage)
    {
        int newPageId = m_engine.createNewDataPage();
        page->nextPage = newPageId;
        page = m_engine.getDataPage(newPageId);
    }

    // write record
    page->rows.push_back(row);
}
} // namespace DBCPP_Storage