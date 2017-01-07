#ifndef __TANG_COMMENT_REMOVAL__
#define __TANG_COMMENT_REMOVAL__
#include <string>
#include "tang_file_reader.hpp"


// NOTE: removes comments, while maintaining line endings.
//       Also converts all line endings to "\n" so all other
//       code can be simplified.

struct Tang_Text_Without_Comments
{
    std::string text;
    
    void from(Tang_File_Reader &source);
};

#endif
