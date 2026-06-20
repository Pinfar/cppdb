#include "where_operator.h"

namespace DBCPP_Operators
{
bool WhereOperator::Next()
{
    while (true)
    {
        bool hasAnyValue = innerOperator->Next();
        if (!hasAnyValue)
            return false;
        current = innerOperator->Current();
        DataCell result = condition->Evaluate(current.get());
        bool valueMatches = std::any_cast<bool>(result.value);
        if (valueMatches)
            return true;
    }
}

std::unique_ptr<DataRow> WhereOperator::Current()
{
    return std::move(current);
}

TableDefinition WhereOperator::GetMetadata()
{
    return innerOperator->GetMetadata();
}
} // namespace DBCPP_Operators