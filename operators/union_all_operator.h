#pragma once
#include "abstract_iterator.h"
#include "execution_plan_base.h"
#include <memory>
#include <utility>

namespace DBCPP_Operators
{
class UnionAllOperator : public AbstractDbOperator
{
  private:
    DbOperator_Ptr m_l_parent;
    DbOperator_Ptr m_r_parent;
    bool m_is_l_active;

  public:
    UnionAllOperator(DbOperator_Ptr &l_parent, DbOperator_Ptr &r_parent)
        : m_l_parent{std::move(l_parent)}, m_r_parent{std::move(r_parent)} {};
    auto Next() -> bool override;
    auto Current() -> std::unique_ptr<DataRow> override;
    auto GetOutputSchema() -> QuerySchema override;
    void Reset() override;
};

class UnionAllExecutionPlanNode : public ExecutionPlanNodeBase
{
  private:
    ExecutionPlanNode_ptr m_r_parent;
    ExecutionPlanNode_ptr m_l_parent;

  public:
    UnionAllExecutionPlanNode(ExecutionPlanNode_ptr &l_parent, ExecutionPlanNode_ptr &r_parent)
        : m_r_parent{std::move(r_parent)}, m_l_parent(std::move(l_parent))
    {
    }
    auto Translate(TranslateContext *context) -> DbOperator_Ptr override;
};
} // namespace DBCPP_Operators