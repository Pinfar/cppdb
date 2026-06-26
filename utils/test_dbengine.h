#pragma once
#include "../sql_interface/parser.h"
#include "../storage/storage.h"

namespace DBCPP::Utils
{
class TestDbEngine
{
  private:
    std::unique_ptr<StorageEngine> m_engine;

    std::unique_ptr<StorageEngine> InitStorage();

  public:
    TestDbEngine();
    StorageEngine *GetStorage();
    DBCPP::SqlInterface::Select_ptr CompileSql(std::string &sql);
    std::string RunSql(std::string &sql);
};
} // namespace DBCPP::Utils