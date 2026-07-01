#include "single_row_operator.h"
#include "metadata/cell.h"
#include "operators/abstract_iterator.h"
#include <memory>

namespace DBCPP_Operators
{
auto SingleRowOperator::Next() -> bool
{
    if (!m_next_done)
    {
        m_next_done = true;
        return true;
    }
    return false;
}

auto SingleRowOperator::Current() -> std::unique_ptr<DataRow>
{
    return std::make_unique<DataRow>();
}

void SingleRowOperator::Reset()
{
    m_next_done = false;
}

auto SingleRowOperator::GetOutputSchema()->QuerySchema{
    return {};
}

auto SingleRowPlanNode::Translate([[maybe_unused]] TranslateContext *context) -> DbOperator_Ptr
{
    return std::make_unique<SingleRowOperator>();
}

} // namespace DBCPP_Operators