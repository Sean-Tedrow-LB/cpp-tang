#ifndef __TANG_FILE_READER__
#define __TANG_FILE_READER__

#include <string>


struct Tang_File_Reader
{
    std::string text_buffer;
    int buffer_progress;
    
    bool open(const std::string &path);
    const char* get() const
    {
        return text_buffer.data() + buffer_progress;
    }
    int length_left() const
    {
        return text_buffer.length() - buffer_progress;
    }
    void advance(int distance)
    {
        buffer_progress += distance;
    }
};


#endif
