#pragma once
#include "metadata/cell.h"
#include <memory>
namespace DBCPP_Operators
{
struct QuerySchema
{
    std::vector<Column> columns;
};

class AbstractDbOperator
{
  public:
    virtual auto Next() -> bool = 0;
    virtual auto Current() -> std::unique_ptr<DataRow> = 0;
    virtual void Reset() = 0;
    virtual ~AbstractDbOperator() = default;
    virtual auto GetOutputSchema() -> QuerySchema = 0;
};

using DbOperator_Ptr = std::unique_ptr<AbstractDbOperator>;

class BaseDbOperator : public AbstractDbOperator
{
  protected:
    DbOperator_Ptr m_innerOperator;

  public:
    BaseDbOperator(DbOperator_Ptr innerOperator) : m_innerOperator{std::move(innerOperator)} {};
    virtual void Reset() override
    {
        m_innerOperator->Reset();
    }
    auto GetOutputSchema() -> QuerySchema override
    {
        return m_innerOperator->GetOutputSchema();
    }
};
} // namespace DBCPP_Operators