#include "where_operator.h"

namespace DBCPP_Operators
{
bool WhereOperator::Next()
{
    while (true)
    {
        bool hasAnyValue = m_innerOperator->Next();
        if (!hasAnyValue)
            return false;
        current = m_innerOperator->Current();
        DataCell result = m_condition->Evaluate(current.get());
        bool valueMatches = std::any_cast<bool>(result.value);
        if (valueMatches)
            return true;
    }
}

std::unique_ptr<DataRow> WhereOperator::Current()
{
    return std::move(current);
}

auto FilterExecutionPlanNode::Translate(TranslateContext *context) -> DbOperator_Ptr
{
    auto parentNode = m_parent->Translate(context);
    return std::make_unique<WhereOperator>(parentNode, m_condition);
}

} // namespace DBCPP_Operators