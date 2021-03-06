#ifndef FILE_H
#define FILE_H

#include <cstdio>
#include <cstdint>

class File
{
public:

    enum class OpenMode
    {
        ReadOnly = 0b0001,
        ReadWrite = 0b0010,
        Append = 0b0100,
        Binary = 0b1000
    };

    File();
    File(const char* filePath);
    ~File();

    void setPath(const char* filePath);

    void open(OpenMode openMode);
    void open(const char* filePath, OpenMode openMode);

    void close();

    void seek(uint64_t pos);

    uint64_t read(char* buffer, uint64_t length);
    uint64_t read(char *buffer, uint64_t length, uint64_t from);

    uint64_t write(const char* data, uint64_t length);

    char readByte();
    char readByte(uint64_t pos);
    void writeByte(char byte);

    int64_t readLongLong();
    void writeLongLong(int64_t n);

    void flush();

    uint64_t getSize();

    inline bool isOpen() const { return (bool)m_pStream; }



private:
    void clearPath();
    void checkSize();


private:
    FILE* m_pStream;
    char* m_path;
    uint64_t m_size;
    char* m_pBuffer;
    size_t m_bufferSize;


};

#endif // FILE_H
