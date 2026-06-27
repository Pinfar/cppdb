#pragma once
#include "../metadata/cell.h"
#include "storage.h"

namespace DBCPP_Storage
{

struct DBWriterOptions
{
    size_t maxRecordsPerPage;
};

class DBWriter
{
  private:
    StorageEngine &m_engine;
    int nextTableId = 1;
    DBWriterOptions m_options;

  public:
    DBWriter(StorageEngine &engine, DBWriterOptions options) : m_engine(engine), m_options(options){};
    void CreateTable(TableDefinition definition);
    void InsertRecord(std::string tableName, DataRow row);
};
} // namespace DBCPP_Storage