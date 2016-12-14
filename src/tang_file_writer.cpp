#include "tang_file_writer.hpp"

#include <iostream>
#include <cerrno>
#include <cstring>

#define TANG_WRITE_BUFFER_SIZE 16000

Tang_File_Writer::Tang_File_Writer()
{
    file = nullptr;
    text_buffer.reserve(TANG_WRITE_BUFFER_SIZE);
}
Tang_File_Writer::~Tang_File_Writer()
{
    if(file)
    {
        fwrite(text_buffer.data(), sizeof(char), text_buffer.length(), file);
        fclose(file);
    }
}

#ifdef _WIN32
#include "ix_unicode.hpp"


static std::FILE* do_fopen(const char *path)
{
    int length = (int)strlen(path);
    std::u16string wpath;
    int wlength = ix_measure_utf8_to_utf16(path, length);
    wpath.resize(wlength);
    char16_t *wstr = &(wpath[0]);
    ix_convert_utf8_to_utf16(wstr, path, length);
    return _wfopen((wchar_t*)wstr, L"w");
}

#else

#define do_fopen(path)    fopen(path, "w")

#endif


bool Tang_File_Writer::open(const char *path)
{
    if(file)
    {
        fwrite(text_buffer.data(), sizeof(char), text_buffer.length(), file);
        fclose(file);
    }
    file = do_fopen(path);
    if(!file)
    {
        std::cout << "Failed to open file \"" << path;
        std::cout << "\" with error \"" << std::strerror(errno) << "\"" << std::endl;
        return false;
    }
    text_buffer.clear();
    return true;
}
void Tang_File_Writer::write(const std::string &input)
{
    text_buffer += input;
    if(text_buffer.length() >= TANG_WRITE_BUFFER_SIZE)
    {
        fwrite(text_buffer.data(), sizeof(char), text_buffer.length(), file);
        text_buffer.clear();
    }
}
