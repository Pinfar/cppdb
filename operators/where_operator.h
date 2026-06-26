#pragma once
#include "../storage/storage.h"
#include "abstract_iterator.h"
#include "expressions.h"
#include "operators/execution_plan_base.h"
#include <memory>

namespace DBCPP_Operators
{
class WhereOperator : public BaseDbOperator
{
  private:
    ExprOper_ptr condition;
    std::unique_ptr<DataRow> current;

  public:
    WhereOperator(std::unique_ptr<AbstractDbOperator> &innerOperator, ExprOper_ptr &condition)
        : BaseDbOperator(std::move(innerOperator)), condition{std::move(condition)} {};
    WhereOperator(WhereOperator &) = delete;
    virtual bool Next() override;
    virtual std::unique_ptr<DataRow> Current() override;
};

class FilterExecutionPlanNode : public ExecutionPlanNodeBase
{
  private:
    ExprOper_ptr m_condition;
    ExecutionPlanNode_ptr m_parent;

  public:
    FilterExecutionPlanNode(ExecutionPlanNode_ptr &parent, ExprOper_ptr &condition)
        : ExecutionPlanNodeBase(), m_condition{std::move(condition)}, m_parent{std::move(parent)}
    {
    }
    auto Translate(TranslateContext *context) -> DbOperator_Ptr override;
};

} // namespace DBCPP_Operators