#include "tang_file_reader.hpp"

#include <iostream>
#include <cerrno>
#include <cstring>

#define TANG_READ_BUFFER_SIZE 16000

Tang_File_Reader::Tang_File_Reader()
{
    file = nullptr;
    text_buffer.reserve(TANG_READ_BUFFER_SIZE);
}
Tang_File_Reader::~Tang_File_Reader()
{
    if(file)
    {
        fclose(file);
    }
}

bool Tang_File_Reader::open(const char *path)
{
    if(file)
    {
        fclose(file);
    }
    file = fopen(path, "r");
    if(!file)
    {
        std::cout << "Failed to open file \"" << path;
        std::cout << "\" with error \"" << std::strerror(errno) << "\"\n";
        return false;
    }
    text_buffer.resize(TANG_READ_BUFFER_SIZE);
    text_buffer.resize(fread(&(text_buffer[0]), sizeof(char), 
                             (size_t)TANG_READ_BUFFER_SIZE, file));
    buffer_progress = 0;
    return true;
}
char Tang_File_Reader::get() const
{
    return text_buffer[buffer_progress];
}
bool Tang_File_Reader::next()
{
    buffer_progress++;
    if(buffer_progress >= TANG_READ_BUFFER_SIZE)
    {
        text_buffer.resize(TANG_READ_BUFFER_SIZE);
        text_buffer.resize(fread(&(text_buffer[0]), sizeof(char), 
                             (size_t)TANG_READ_BUFFER_SIZE, file));
        buffer_progress = 0;
    }
    return buffer_progress < (int)text_buffer.length();
}
