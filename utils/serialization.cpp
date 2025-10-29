#include "serialization.h"

std::string DeserializeAsString(DataCell& cell){
    std::string result{
        cell.bytes.begin(), cell.bytes.end()
    };
    return result;
}

int DeserializeAsInt(DataCell& cell){
    auto bytes = cell.bytes;
    int value = int((unsigned char)(bytes[3]) << 24 |
            (unsigned char)(bytes[2]) << 16 |
            (unsigned char)(bytes[1]) << 8 |
            (unsigned char)(bytes[0]));
    return value;
}

DataCell Serialize(std::string& value){
    std::vector<unsigned char> serialized{value.begin(), value.end()};
    return {(int)serialized.size(), serialized};
}

DataCell Serialize(int value){
    auto as_bytes = static_cast<const char*>(static_cast<const void*>(&value));
    std::vector<unsigned char> serialized{as_bytes, as_bytes + sizeof(int)};
    return {(int)serialized.size(), serialized};
}