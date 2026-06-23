#include "type_interface.h"
#include "type_base.h"

namespace DBCPP::Types
{
auto BinaryOperatorKey::operator<(const BinaryOperatorKey &rhs) const -> bool
{
    // we treat BinaryOperatorKey as long number in format (lhsType rhsType token)

    if (lhsType < rhs.lhsType)
    {
        return true;
    }
    if (lhsType > rhs.lhsType)
    {
        return false;
    }

    if (rhsType < rhs.rhsType)
    {
        return true;
    }
    if (rhsType > rhs.rhsType)
    {
        return false;
    }

    if (token < rhs.token)
    {
        return true;
    }
    if (token > rhs.token)
    {
        return false;
    }

    return false; // they are equal
}

#define register_op(type1, type2, token, returnType)                                                                   \
    m_operator_registry[{ColumnType::type1, ColumnType::type2, TokenType::token}] = {type1##type2##token,              \
                                                                                     ColumnType::returnType}

TypeResolver::TypeResolver()
{
    using namespace SqlInterface;

    register_op(Int, Int, Plus, Int);
    register_op(Int, Int, Minus, Int);
    register_op(Int, Int, Star, Int);
    register_op(Int, Int, Slash, Int);

    register_op(Int, Int, Eq, Boolean);
    register_op(Int, Int, Neq, Boolean);
    register_op(Int, Int, Gt, Boolean);
    register_op(Int, Int, Lt, Boolean);

    register_op(String, String, Plus, String);
    register_op(String, String, Eq, Boolean);
    register_op(String, String, Neq, Boolean);
    register_op(String, String, Gt, Boolean);
    register_op(String, String, Lt, Boolean);

    register_op(Boolean, Boolean, And, Boolean);
    register_op(Boolean, Boolean, Or, Boolean);
}

auto TypeResolver::ResolveBinaryOperator(BinaryOperatorKey key) -> BinaryOperatorValue
{
    return m_operator_registry[key];
}

} // namespace DBCPP::Types