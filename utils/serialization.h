#pragma once
#include<string>
#include"../metadata/cell.h"


std::string DeserializeAsString(std::vector<unsigned char>& bytes);
int DeserializeAsInt(std::vector<unsigned char>& bytes);
std::vector<unsigned char> Serialize(std::string value);
std::vector<unsigned char> Serialize(int value);