#ifndef __TANG_BLOCK__
#define __TANG_BLOCK__

#include <vector>
#include <map>
#include "tang_name.cpp"

struct Tang_Statement_Data
{
    const char *start;
    int length, line_number;
};


union Tang_Child_Data
{
    Tang_Statement_Data statement;
    Tang_Block *block;
};

enum Tang_Block_Child_Type
{
    TANG_CHILD_STATEMENT,
    TANG_CHILD_BLOCK
};

struct Tang_Block_Child
{
    Tang_Block_Child_Type type;
    Tang_Child_Data data;
};

struct Tang_Block
{
    std::vector<Tang_Block_Child> children;
    std::map<std::string, Tang_Name_Base*> name_map;
    int indentation;
    
    ~Tang_Block(); // TODO: iterate over and delete names
};

#endif
