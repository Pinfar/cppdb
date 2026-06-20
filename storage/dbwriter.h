#pragma once
#include "../metadata/cell.h"
#include "storage.h"

namespace DBCPP_Storage
{

struct DBWriterOptions
{
    int maxRecordsPerPage;
};

class DBWriter
{
  private:
    StorageEngine &engine;
    int nextTableId = 1;
    DBWriterOptions options;

  public:
    DBWriter(StorageEngine &engine, DBWriterOptions options) : engine(engine), options(options){};
    void CreateTable(TableDefinition definition);
    void InsertRecord(std::string tableName, DataRow row);
};
} // namespace DBCPP_Storage