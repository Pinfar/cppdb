#include "projection_operator.h"
#include "metadata/cell.h"
#include "operators/abstract_iterator.h"

#include <memory>
#include <vector>

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

auto ProjectionOperator::GetOutputSchema() -> QuerySchema
{
    std::vector<Column> columns;
    for (size_t i = 0; i < m_columns.size(); i++)
    {
        columns.push_back({m_columnNames[i], m_columns[i]->GetType()});
    }
    return {columns};
}

auto ProjectionExecutionPlanNode::Translate(TranslateContext *context) -> DbOperator_Ptr
{
    auto parentNode = m_parent->Translate(context);
    return std::make_unique<ProjectionOperator>(parentNode, m_columns, m_columnNames);
}

} // namespace DBCPP_Operators