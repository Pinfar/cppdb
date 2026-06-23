#pragma once
#include "../metadata/cell.h"
#include "map"
#include "sql_interface/scanner.h"

namespace DBCPP::Types
{
using BinaryTypeOperator = DataCell (*)(DataCell &, DataCell &);
struct BinaryOperatorKey
{
    ColumnType lhsType;
    ColumnType rhsType;
    SqlInterface::TokenType token;
    auto operator<(const BinaryOperatorKey &rhs) const -> bool;
};

struct BinaryOperatorValue
{
  BinaryTypeOperator func;
  ColumnType returnType;
};

class TypeResolver
{
  private:
    std::map<BinaryOperatorKey, BinaryOperatorValue> m_operator_registry;

  public:
    TypeResolver();
    auto ResolveBinaryOperator(BinaryOperatorKey key) -> BinaryOperatorValue;
};
} // namespace DBCPP::Types