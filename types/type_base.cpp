#include "type_base.h"
#include "metadata/cell.h"
#include <any>

namespace DBCPP::Types
{

// int

#define INT_OP(sign)                                                                                                   \
    {                                                                                                                  \
        std::any_cast<int>(lhs.value) sign std::any_cast<int>(rhs.value)                                               \
    }

auto IntIntPlus(DataCell &lhs, DataCell &rhs) -> DataCell
{
    return INT_OP(+);
}

auto IntIntMinus(DataCell &lhs, DataCell &rhs) -> DataCell
{
    return INT_OP(-);
}

auto IntIntStar(DataCell &lhs, DataCell &rhs) -> DataCell
{
    return INT_OP(*);
}

auto IntIntSlash(DataCell &lhs, DataCell &rhs) -> DataCell
{
    return INT_OP(/);
}

auto IntIntEq(DataCell &lhs, DataCell &rhs) -> DataCell
{
    return INT_OP(==);
}
auto IntIntNeq(DataCell &lhs, DataCell &rhs) -> DataCell
{
    return INT_OP(!=);
}
auto IntIntGt(DataCell &lhs, DataCell &rhs) -> DataCell
{
    return INT_OP(>);
}
auto IntIntLt(DataCell &lhs, DataCell &rhs) -> DataCell
{
    return INT_OP(<);
}

// string
#define STRING_OP(sign)                                                                                                \
    {                                                                                                                  \
        std::any_cast<std::string>(lhs.value) sign std::any_cast<std::string>(rhs.value)                               \
    }

auto StringStringPlus(DataCell &lhs, DataCell &rhs) -> DataCell
{
    return STRING_OP(+);
}

auto StringStringEq(DataCell &lhs, DataCell &rhs) -> DataCell
{
    return STRING_OP(==);
}

auto StringStringNeq(DataCell &lhs, DataCell &rhs) -> DataCell
{
    return STRING_OP(!=);
}

auto StringStringGt(DataCell &lhs, DataCell &rhs) -> DataCell
{
    return STRING_OP(>);
}

auto StringStringLt(DataCell &lhs, DataCell &rhs) -> DataCell
{
    return STRING_OP(<);
}

// bool
#define BOOL_OP(sign)                                                                                                  \
    {                                                                                                                  \
        std::any_cast<bool>(lhs.value) sign std::any_cast<bool>(rhs.value)                                             \
    }

auto BooleanBooleanAnd(DataCell &lhs, DataCell &rhs) -> DataCell
{
    return BOOL_OP(&&);
}

auto BooleanBooleanOr(DataCell &lhs, DataCell &rhs) -> DataCell
{
    return BOOL_OP(||);
}

} // namespace DBCPP::Types