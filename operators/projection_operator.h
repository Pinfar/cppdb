#pragma once
#include "abstract_iterator.h"
#include "execution_plan_base.h"
#include "operators/expressions.h"
#include <memory>
#include <utility>

namespace DBCPP_Operators
{
class ProjectionOperator : public BaseDbOperator
{
  private:
    std::vector<ExprOper_ptr> m_columns;

  public:
    ProjectionOperator(std::unique_ptr<AbstractDbOperator> &innerOperator, std::vector<ExprOper_ptr> &columns)
        : BaseDbOperator(std::move(innerOperator)), m_columns{std::move(columns)} {};
    ProjectionOperator(ProjectionOperator &) = delete;
    virtual bool Next() override;
    virtual std::unique_ptr<DataRow> Current() override;
};

class ProjectionExecutionPlanNode : public ExecutionPlanNodeBase
{
  private:
    std::vector<ExprOper_ptr> m_columns;
    ExecutionPlanNode_ptr m_parent;

  public:
    ProjectionExecutionPlanNode(ExecutionPlanNode_ptr &parent, std::vector<ExprOper_ptr> &columns)
        : ExecutionPlanNodeBase(), m_columns{std::move(columns)}, m_parent(std::move(parent))
    {
    }
    auto Translate(TranslateContext *context) -> DbOperator_Ptr override;
};
} // namespace DBCPP_Operators