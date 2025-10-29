#pragma once
#include<string>
#include"../metadata/cell.h"

std::string GetSerializedHeader(TableDefinition& table);
std::string GetSerializedRow(DataRow& row, TableDefinition& table);
std::string GetStringValue(DataCell& cell, Column& columnData );