#pragma once
#include<string>
#include"../metadata/cell.h"

std::string GetSerializedRow(DataRow& row, TableDefinition& table);
std::string GetStringValue(DataCell& cell, Column& columnData );
std::string DeserializeAsString(DataCell& cell);
int DeserializeAsInt(DataCell& cell);