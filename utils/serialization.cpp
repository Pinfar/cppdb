#include "serialization.h"

std::string DeserializeAsString(std::vector<unsigned char> &bytes)
{
    std::string result{bytes.begin(), bytes.end()};
    return result;
}

int DeserializeAsInt(std::vector<unsigned char> &bytes)
{
    int value = int((unsigned char)(bytes[3]) << 24 | (unsigned char)(bytes[2]) << 16 | (unsigned char)(bytes[1]) << 8 |
                    (unsigned char)(bytes[0]));
    return value;
}

std::vector<unsigned char> Serialize(std::string value)
{
    std::vector<unsigned char> serialized{value.begin(), value.end()};
    return serialized;
}

std::vector<unsigned char> Serialize(int value)
{
    auto as_bytes = static_cast<const char *>(static_cast<const void *>(&value));
    std::vector<unsigned char> serialized{as_bytes, as_bytes + sizeof(int)};
    return serialized;
}