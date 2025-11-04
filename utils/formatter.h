#pragma once
#include<string>
#include"../metadata/cell.h"
#include"../operators/abstract_iterator.h"

std::string GetSerializedHeader(TableDefinition& table);
std::string GetSerializedRow(std::unique_ptr<DataRow>& row);
std::string GetStringValue(DataCell& cell);
std::string GetSerializedResult(std::vector<std::unique_ptr<DataRow>>& result, TableDefinition& table);
std::string GetSerializedOpearatorOutput(DBCPP_Operators::AbstractDbOperator& oper, TableDefinition& table);