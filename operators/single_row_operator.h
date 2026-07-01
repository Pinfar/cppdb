#pragma once
#include "abstract_iterator.h"
#include "execution_plan_base.h"
#include <memory>

namespace DBCPP_Operators
{
class SingleRowOperator : public AbstractDbOperator
{
  private:
    bool m_next_done = false;

  public:
    SingleRowOperator() = default;
    SingleRowOperator(SingleRowOperator &) = delete;
    auto Next() -> bool override;
    auto Current() -> std::unique_ptr<DataRow> override;
    void Reset() override;
    auto GetOutputSchema() -> QuerySchema override;
};

class SingleRowPlanNode : public ExecutionPlanNodeBase
{
  public:
    SingleRowPlanNode() = default;
    auto Translate(TranslateContext *context) -> DbOperator_Ptr override;
};
} // namespace DBCPP_Operators