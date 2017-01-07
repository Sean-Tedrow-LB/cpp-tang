#include "tang_file_reader.hpp"

#include <iostream>
#include <cerrno>
#include <cstring>
#include "ix_unicode.hpp"
#include <cstdio>


#define TANG_READ_BUFFER_SIZE 16000

#ifdef _WIN32


inline std::FILE* do_fopen(const char *path, int length)
{
    std::u16string wpath;
    int wlength = ix_measure_utf8_to_utf16(path, length);
    wpath.resize(wlength);
    char16_t *wstr = &(wpath[0]);
    ix_convert_utf8_to_utf16(wstr, path, length);
    return _wfopen((wchar_t*)wstr, L"r");
}

#else

#define do_fopen(path, length)    fopen(path, "r")

#endif

static bool check_for_bom(const char *to_check)
{
    for(int i = 0; i < IX_UTF8_BOM_SIZE; i++)
    {
        if(to_check[i] != IX_UTF8_BOM[i])
        {
            return false;
        }
    }
    return true;
}

bool Tang_File_Reader::open(const std::string &path)
{
    FILE *file = do_fopen(path.c_str(), path.length());
    if(!file)
    {
        std::cout << "Failed to open file \"" << path;
        std::cout << "\" with error \"" << std::strerror(errno) << "\"\n";
        return false;
    }
    size_t read_size = 0;
    std::string::size_type old_length = 0;
    do
    {
        old_length = text_buffer.length();
        text_buffer.resize(old_length + TANG_READ_BUFFER_SIZE);
        read_size = fread(&(text_buffer[old_length]), sizeof(char), 
                          (size_t)TANG_READ_BUFFER_SIZE, file);
    }
    while(read_size == TANG_READ_BUFFER_SIZE);
    text_buffer.resize(old_length + read_size);
    buffer_progress = 0;
    if(text_buffer.length() >= IX_UTF8_BOM_SIZE)
    {
        if(check_for_bom(text_buffer.data()))
        {
            buffer_progress = IX_UTF8_BOM_SIZE;
        }
    }
    return true;
}
