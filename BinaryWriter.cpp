// json_packer assignment
// Author: Ilya Fateev

#include "BinaryWriter.h"

BinaryWriter::BinaryWriter(const std::string &fileName)
{
    m_ofstream = std::ofstream(fileName, std::ios::out | std::ios::binary);
}

BinaryWriter::~BinaryWriter()
{

}

template <> uint8_t BinaryWriter::GetTypeByte<bool>()
{
    return (uint8_t)TypesEnum::Boolean;
}

template <> uint8_t BinaryWriter::GetTypeByte<int>()
{
    return (uint8_t)TypesEnum::Integer;
}

template <> uint8_t BinaryWriter::GetTypeByte<std::string>()
{
    return (uint8_t)TypesEnum::String;
}

template <> uint8_t BinaryWriter::GetTypeByte<float>()
{
    return (uint8_t)TypesEnum::Float;
}

template <> size_t BinaryWriter::GetLength<std::string>(const std::string& value)
{
    return value.size();
}

template <> void BinaryWriter::WriteValue<bool>(const bool &value)
{
    WriteIntegral(value);
}

template <> void BinaryWriter::WriteValue<int>(const int &value)
{
    int32_t networkInt = htonl(value);
    WriteIntegral(networkInt);
}

template <> void BinaryWriter::WriteValue<std::string>(const std::string &value)
{
    m_ofstream.write(value.c_str(), value.size());
}

template <> void BinaryWriter::WriteValue<float>(const float &value)
{
    union {
        float floatValue;
        int32_t integerValue;
    } u;
    u.floatValue = value;
    int32_t networkInt = htonl(u.integerValue);
    WriteIntegral(networkInt);
}

void BinaryWriter::WriteNullElement(int key)
{
    uint32_t networkKeyInt = htonl(key);
    WriteIntegral(networkKeyInt);

    uint8_t typeByte = (uint8_t)TypesEnum::Null;
    WriteIntegral(typeByte);
}

void BinaryWriter::WriteDictionary(Dictionary &dictionary)
{
    for (auto it = dictionary.begin(); it != dictionary.end(); ++it) {
        std::string keyString = (*it).first;
        int keyInteger = (*it).second;

        WriteValue(keyInteger);

        uint16_t networkLength = htons(GetLength(keyString));
        WriteIntegral(networkLength);

        WriteValue(keyString);
    }
}

