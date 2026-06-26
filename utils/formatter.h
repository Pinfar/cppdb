#pragma once
#include "../metadata/cell.h"
#include "../operators/abstract_iterator.h"
#include <string>

std::string GetSerializedHeader(std::vector<std::string> &columns);
std::string GetSerializedRow(std::unique_ptr<DataRow> &row);
std::string GetStringValue(DataCell &cell);
std::string GetSerializedResult(std::vector<std::unique_ptr<DataRow>> &result, std::vector<std::string> &columns);
std::string GetSerializedOpearatorOutput(DBCPP_Operators::AbstractDbOperator &oper, std::vector<std::string> &columns);