#ifndef __TANG_FILE_READER__
#define __TANG_FILE_READER__

#include <string>


struct Tang_File_Reader
{
    std::FILE *file;
    std::string text_buffer;
    int buffer_progress;
    
    
    Tang_File_Reader();
    ~Tang_File_Reader();
    
    bool open(const char *path);
    char get() const;
    bool next();
};


#endif
