#pragma once
#include "../storage/storage.h"
#include <memory>
namespace DBCPP_Operators
{
class AbstractDbOperator
{
  public:
    virtual bool Next() = 0;
    virtual std::unique_ptr<DataRow> Current() = 0;
    virtual void Reset() = 0;
    virtual ~AbstractDbOperator(){};
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
};
} // namespace DBCPP_Operators