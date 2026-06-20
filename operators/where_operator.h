#pragma once
#include "../storage/storage.h"
#include "abstract_iterator.h"
#include "expressions.h"
#include <memory>

namespace DBCPP_Operators
{
class WhereOperator : public BaseDbOperator
{
  private:
    ExprOper_ptr condition;
    std::unique_ptr<DataRow> current;

  public:
    WhereOperator(std::unique_ptr<AbstractDbOperator> innerOperator, ExprOper_ptr condition)
        : BaseDbOperator(std::move(innerOperator)), condition{std::move(condition)} {};
    WhereOperator(WhereOperator &) = delete;
    virtual bool Next() override;
    virtual std::unique_ptr<DataRow> Current() override;
    virtual TableDefinition GetMetadata() override;
};
} // namespace DBCPP_Operators