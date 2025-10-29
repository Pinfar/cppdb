#pragma once
#include<string>
#include"../metadata/cell.h"


std::string DeserializeAsString(DataCell& cell);
int DeserializeAsInt(DataCell& cell);
DataCell Serialize(std::string& value);
DataCell Serialize(int value);