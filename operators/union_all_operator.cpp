#include "union_all_operator.h"
#include <memory>
namespace DBCPP_Operators
{
auto UnionAllOperator::Next() -> bool
{
    if (m_is_l_active)
    {
        m_is_l_active = m_l_parent->Next();
        if (m_is_l_active)
        {
            return true;
        }
    }
    return m_r_parent->Next();
}

auto UnionAllOperator::Current() -> std::unique_ptr<DataRow>
{
    if (m_is_l_active)
    {
        return m_l_parent->Current();
    }
    return m_r_parent->Current();
}

auto UnionAllOperator::GetOutputSchema() -> QuerySchema
{
    return m_l_parent->GetOutputSchema();
}

void UnionAllOperator::Reset()
{
    m_is_l_active = true;
    m_l_parent->Reset();
    m_r_parent->Reset();
}
auto UnionAllExecutionPlanNode::Translate(TranslateContext *context) -> DbOperator_Ptr
{
    auto left = m_l_parent->Translate(context);
    auto right = m_r_parent->Translate(context);
    return std::make_unique<UnionAllOperator>(left, right);
}
} // namespace DBCPP_Operators