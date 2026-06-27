#pragma once
#include "../storage/storage.h"
#include "abstract_iterator.h"
#include "operators/execution_plan_base.h"
#include <string>
namespace DBCPP_Operators
{
class FullScanOperator : public AbstractDbOperator
{
  private:
    StorageEngine *m_storageEngine;
    std::string m_tableName;
    DataPage *currentPage;
    int currentPosition = -1;
    TableHeader metadata;

  public:
    FullScanOperator(StorageEngine *storageEngine, std::string tableName);
    FullScanOperator(FullScanOperator &) = delete;
    virtual bool Next() override;
    virtual std::unique_ptr<DataRow> Current() override;
    virtual void Reset() override;
};

class FullScanExecutionPlanNode : public ExecutionPlanNodeBase
{
  private:
    std::string m_tableName;

  public:
    FullScanExecutionPlanNode(std::string &tableName) : ExecutionPlanNodeBase(), m_tableName{tableName}
    {
    }
    auto Translate(TranslateContext *context) -> DbOperator_Ptr override;
};

} // namespace DBCPP_Operators