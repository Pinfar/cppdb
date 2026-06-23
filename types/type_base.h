#pragma once
#include "../metadata/cell.h"

namespace DBCPP::Types
{

// INT MATH
auto IntIntPlus(DataCell &, DataCell &) -> DataCell;
auto IntIntMinus(DataCell &, DataCell &) -> DataCell;
auto IntIntStar(DataCell &, DataCell &) -> DataCell;
auto IntIntSlash(DataCell &, DataCell &) -> DataCell;

// INT COMPARISON
auto IntIntEq(DataCell &, DataCell &) -> DataCell;
auto IntIntNeq(DataCell &, DataCell &) -> DataCell;
auto IntIntGt(DataCell &, DataCell &) -> DataCell;
auto IntIntLt(DataCell &, DataCell &) -> DataCell;

// string
auto StringStringPlus(DataCell &, DataCell &) -> DataCell;
auto StringStringEq(DataCell &, DataCell &) -> DataCell;
auto StringStringNeq(DataCell &, DataCell &) -> DataCell;
auto StringStringGt(DataCell &, DataCell &) -> DataCell;
auto StringStringLt(DataCell &, DataCell &) -> DataCell;

// bool
auto BooleanBooleanAnd(DataCell &, DataCell &) -> DataCell;
auto BooleanBooleanOr(DataCell &, DataCell &) -> DataCell;
} // namespace DBCPP::Types