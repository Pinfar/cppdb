#include "projection_operator.h"

#include <memory>

namespace DBCPP_Operators
{
bool ProjectionOperator::Next()
{
    return m_innerOperator->Next();
}

std::unique_ptr<DataRow> ProjectionOperator::Current()
{
    auto innerCurrent = m_innerOperator->Current();
    std::vector<DataCell> cells;
    for (auto &column_expr : m_columns)
    {
        cells.push_back(column_expr->Evaluate(innerCurrent.get()));
    }
    return std::make_unique<DataRow>(DataRow{cells});
}

auto ProjectionExecutionPlanNode::Translate(TranslateContext *context) -> DbOperator_Ptr
{
    auto parentNode = m_parent->Translate(context);
    return std::make_unique<ProjectionOperator>(parentNode, m_columns);
}

} // namespace DBCPP_Operators