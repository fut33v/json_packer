// json_packer assignment
// Author: Ilya Fateev

#include <fstream>
#include <filesystem>
#include <gtest/gtest.h>
#include "BinaryWriter.h"

template <typename T>
T Read(std::ifstream &ifstream) {
    T value;
    ifstream.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

float IntToFloat(int value) {
    union {
        int intValue;
        float floatValue;
    } u;
    u.intValue = value;
    return u.floatValue;
}

TEST(BinaryWriterTest, WriteReadRecord) {

    std::string fname = "test.bin";

    auto writer = std::make_unique<BinaryWriter>(fname);
    writer->WriteRecordElement(3, true);
    writer->WriteRecordElement(4, 12);
    writer->WriteRecordElement(6, std::string("somestring"));
    writer->WriteRecordElement(9, 69.420f);
    writer->WriteNullElement(7);
    writer.reset();

    std::ifstream binaryFile(fname, std::ios::binary);

    int key;
    uint8_t type;
    uint16_t length;
    bool boolValue;
    int intValue;
    float floatValue;

    key = Read<int>(binaryFile);
    key = ntohl(key);
    EXPECT_EQ(key, 3);

    type = Read<uint8_t>(binaryFile);
    EXPECT_EQ(type, (uint8_t)TypesEnum::Boolean);

    length = Read<uint16_t>(binaryFile);
    length = ntohs(length);

    EXPECT_EQ(length, 1);
    boolValue = Read<bool>(binaryFile);
    EXPECT_EQ(boolValue, true);


    key = Read<int>(binaryFile);
    key = ntohl(key);
    EXPECT_EQ(key, 4);

    type = Read<uint8_t>(binaryFile);
    EXPECT_EQ(type, (uint8_t)TypesEnum::Integer);

    length = Read<uint16_t>(binaryFile);
    length = ntohs(length);
    EXPECT_EQ(length, sizeof(int));

    intValue = Read<int>(binaryFile);
    intValue = ntohl(intValue);
    EXPECT_EQ(intValue, 12);


    key = Read<int>(binaryFile);
    key = ntohl(key);
    EXPECT_EQ(key, 6);

    type = Read<uint8_t>(binaryFile);
    EXPECT_EQ(type, (uint8_t)TypesEnum::String);

    length = Read<uint16_t>(binaryFile);
    length = ntohs(length);
    EXPECT_EQ(length, 10);

    std::string stringValue(10, ' ');
    binaryFile.read(&stringValue[0], 10);
    EXPECT_STREQ(stringValue.c_str(), "somestring");


    key = Read<int>(binaryFile);
    key = ntohl(key);
    EXPECT_EQ(key, 9);

    type = Read<uint8_t>(binaryFile);
    EXPECT_EQ(type, (uint8_t)TypesEnum::Float);

    length = Read<uint16_t>(binaryFile);
    length = ntohs(length);
    EXPECT_EQ(length, sizeof(int));

    intValue = Read<int>(binaryFile);
    intValue = ntohl(intValue);
    EXPECT_EQ(IntToFloat(intValue), 69.420f);

    std::filesystem::remove(fname);
}
