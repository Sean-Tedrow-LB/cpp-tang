#ifndef __TANG_FILE_WRITER__
#define __TANG_FILE_WRITER__

#include <string>


struct Tang_File_Writer
{
    std::FILE *file;
    std::string text_buffer;
    
    
    Tang_File_Writer();
    ~Tang_File_Writer();
    
    bool open(const char *path);
    void write(const std::string &input);
};


#endif
