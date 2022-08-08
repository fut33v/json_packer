// json_packer assignment
// Author: Ilya Fateev

#include <fstream>
#include <string>

#include <unordered_map>
#include <arpa/inet.h>

template<class T>
concept Integral = std::is_integral<T>::value;

enum class TypesEnum : uint8_t
{
    Boolean,
    Integer,
    String,
    Float
};

// record:
// key (4 bytes) | type (1 byte) | length (2 bytes) | value (length bytes)
// dictionary:
// key (4 bytes) | length (2 bytes) | value (lenght bytes)
class BinaryWriter final
{
    public:

        BinaryWriter(const std::string &fileName);
        ~BinaryWriter();

        BinaryWriter(const BinaryWriter&) = delete;
        BinaryWriter& operator=(BinaryWriter&) = delete;

        template <typename T>
        void WriteRecordElement(int32_t key, T value)
        {
            uint32_t networkKeyInt = htonl(key);
            WriteIntegral(networkKeyInt);

            uint8_t typeByte = GetTypeByte<T>();
            WriteIntegral(typeByte);

            uint16_t networkLength = htons(GetLength<T>(value));
            WriteIntegral(networkLength);

            WriteValue(value);
        }

        void WriteDictionary(std::unordered_map<std::string, int32_t> &dictionary);

    private:

        template<typename T>
        uint8_t GetTypeByte();

        template<typename T>
        uint32_t GetLength(T const &value)
        {
            return sizeof(T);
        }

        template<Integral T>
        void WriteIntegral(T value)
        {
            m_ofstream.write(reinterpret_cast<char*>(&value), sizeof(value));
        }

        template<typename T>
        void WriteValue(T const &value);

        std::ofstream m_ofstream;

};