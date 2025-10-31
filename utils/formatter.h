#pragma once
#include<string>
#include"../metadata/cell.h"
#include"../operators/abstract_iterator.h"

std::string GetSerializedHeader(TableDefinition& table);
std::string GetSerializedRow(DataRow& row, TableDefinition& table);
std::string GetStringValue(DataCell& cell, Column& columnData);
std::string GetSerializedResult(std::vector<DataRow>& result, TableDefinition& table);
std::string GetSerializedOpearatorOutput(DBCPP_Operators::AbstractDbOperator& oper, TableDefinition& table);