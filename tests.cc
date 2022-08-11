// json_packer assignment
// Author: Ilya Fateev

#include <fstream>
#include <filesystem>
#include <gtest/gtest.h>
#include "BinaryWriter.h"
#include "Worker.h"

template <typename T>
T Read(std::ifstream &ifstream)
{
    T value;
    ifstream.read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

float IntToFloat(int value)
{
    union {
        int intValue;
        float floatValue;
    } u;
    u.intValue = value;
    return u.floatValue;
}

TEST(BinaryWriterTest, WriteReadRecords)
{
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

    key = Read<int>(binaryFile);
    key = ntohl(key);
    EXPECT_EQ(key, 7);

    type = Read<uint8_t>(binaryFile);
    EXPECT_EQ(type, (uint8_t)TypesEnum::Null);

    char tempChar;
    binaryFile.get(tempChar);
    EXPECT_EQ(binaryFile.eof(), true);

    std::filesystem::remove(fname);
}

TEST(BinaryWriterTest, WriteReadDictionary)
{
    std::string fname = "test.bin";

    auto writer = std::make_unique<BinaryWriter>(fname);
    Dictionary dictionary = {{"key4", 1}, {"key1", 0}, {"key2", 2}};
    writer->WriteDictionary(dictionary);
    writer.reset();

    std::ifstream binaryFile(fname, std::ios::binary);

    while (!binaryFile.eof()) {
        int key;
        uint16_t length;

        key = Read<int>(binaryFile);
        key = ntohl(key);

        if (binaryFile.eof()) {
            break;
        }

        length = Read<uint16_t>(binaryFile);
        length = ntohs(length);

        std::string stringKey(length, ' ');
        binaryFile.read(&stringKey[0], length);

        EXPECT_TRUE(dictionary.contains(stringKey));

        EXPECT_EQ(dictionary[stringKey], key);
    }

    std::filesystem::remove(fname);
}

TEST(WorkerTest, RunSample)
{
    char fnameBin[] = "tests";
    char fnameIn[]= "../sample.txt";
    char fnameOut[] = "test.bin";
    char *argv[3] = {fnameBin, fnameIn, fnameOut};

    int ret = Worker::Main(3, argv);

    EXPECT_EQ(ret, EXIT_SUCCESS);

    std::filesystem::remove(fnameOut);
}
