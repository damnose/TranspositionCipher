
#include <exception>
#include <cassert>
#include <cstring>


#include "file.h"

File::File()
    : m_path(0)
    , m_size((uint64_t)-1)
    , m_pStream(0)
    , m_bufferSize(4096 * 4)
{
    m_pBuffer = (char*)malloc(m_bufferSize);
}

File::File(const char *filePath)
    : File()
{
    setPath(filePath);
}

File::~File()
{
    clearPath();
    free(m_pBuffer);
}


void File::setPath(const char *filePath)
{
    clearPath();
    size_t length = strlen(filePath);
    m_path = (char*)calloc(length + 1, sizeof(char));
    strcpy(m_path, filePath);
}

void File::clearPath()
{
    if(m_path)
    {
        free(m_path);
        m_path = 0;
    }
}

void File::open(File::OpenMode openMode)
{
    if(!(m_path && strlen(m_path)))
        throw std::exception("file path is empty");

    char openModeStr[5];
    memset(openModeStr, 0, sizeof(openMode));
    openModeStr[1] = '+';

    if((int)openMode & (int)OpenMode::Append)
        openModeStr[0] = 'a';
    else if((int)openMode & (int)OpenMode::ReadWrite)
        openModeStr[0] = 'w';
    else
        openModeStr[0] = 'r';

    if((int)openMode & (int)OpenMode::Binary)
        openModeStr[2] = 'b';

    m_pStream = fopen(m_path, openModeStr);
    if(!m_pStream)
        throw std::exception("file not found");

    setvbuf(m_pStream, m_pBuffer, _IOFBF, m_bufferSize);

    checkSize();
}

void File::open(const char *filePath, File::OpenMode openMode)
{
    setPath(filePath);
    open(openMode);
}

void File::close()
{
    if(m_pStream)
    {
        fclose(m_pStream);
        m_pStream = 0;
        m_size = (uint64_t)-1;
    }
}

void File::seek(uint64_t pos)
{
    _fseeki64(m_pStream, pos, SEEK_SET);
}

uint64_t File::read(char *buffer, uint64_t length)
{
    return fread(buffer, 1, length, m_pStream);
}

uint64_t File::read(char *buffer, uint64_t length, uint64_t from)
{
    this->seek(from);
    return read(buffer, length);
}

uint64_t File::write(const char *data, uint64_t length)
{
    return fwrite(data, 1, length, m_pStream);
}

char File::readByte()
{
    return fgetc(m_pStream);
}

char File::readByte(uint64_t pos)
{
    this->seek(pos);
    return fgetc(m_pStream);
}

void File::writeByte(char byte)
{
    fwrite(&byte, sizeof(char), 1, m_pStream);
}

int64_t File::readLongLong()
{
    int64_t n;
    fread(&n, sizeof(int64_t), 1, m_pStream);
    return n;
}

void File::writeLongLong(int64_t n)
{
    fwrite(&n, sizeof(int64_t), 1, m_pStream);
}

void File::flush()
{
    if(!m_pStream)
        throw std::exception("file flush error");
    fflush(m_pStream);
}

void File::checkSize()
{
    assert(m_pStream);
    uint64_t currentPos = _ftelli64(m_pStream);

    if(!_fseeki64(m_pStream, 0, SEEK_END))
    {
        m_size = _ftelli64(m_pStream);
        _fseeki64(m_pStream, currentPos, SEEK_SET);
    }
}

uint64_t File::getSize()
{
    if(m_size == (uint64_t)-1)
        throw std::exception("file not opened");
    return m_size;
}
