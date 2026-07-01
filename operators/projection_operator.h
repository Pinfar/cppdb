#pragma once
#include "abstract_iterator.h"
#include "execution_plan_base.h"
#include "operators/expressions.h"
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace DBCPP_Operators
{
class ProjectionOperator : public BaseDbOperator
{
  private:
    std::vector<ExprOper_ptr> m_columns;
    std::vector<std::string> m_columnNames;

  public:
    ProjectionOperator(std::unique_ptr<AbstractDbOperator> &innerOperator, std::vector<ExprOper_ptr> &columns,
                       std::vector<std::string> &columnNames)
        : BaseDbOperator(std::move(innerOperator)), m_columns{std::move(columns)},
          m_columnNames{std::move(columnNames)} {};
    ProjectionOperator(ProjectionOperator &) = delete;
    auto Next() -> bool override;
    auto Current() -> std::unique_ptr<DataRow> override;
    auto GetOutputSchema() -> QuerySchema override;
};

class ProjectionExecutionPlanNode : public ExecutionPlanNodeBase
{
  private:
    std::vector<ExprOper_ptr> m_columns;
    ExecutionPlanNode_ptr m_parent;
    std::vector<std::string> m_columnNames;

  public:
    ProjectionExecutionPlanNode(ExecutionPlanNode_ptr &parent, std::vector<ExprOper_ptr> &columns,
                                std::vector<std::string> &columnNames)
        : ExecutionPlanNodeBase(), m_columns{std::move(columns)}, m_parent(std::move(parent)),
          m_columnNames{std::move(columnNames)}
    {
    }
    auto Translate(TranslateContext *context) -> DbOperator_Ptr override;
};
} // namespace DBCPP_Operators