#ifndef __TANG_BLOCK_TEMPLATE__
#define __TANG_BLOCK_TEMPLATE__

struct Tang_Block_Template;

#include <vector>
#include <map>
#include "tang_name.hpp"

struct Tang_Statement_Data
{
    const char *start;
    int length, line_number;
};

struct Tang_Block_Template;

union Tang_Block_Template_Child_Data
{
    Tang_Statement_Data  statement;
    Tang_Block_Template *block;
};

enum Tang_Block_Template_Child_Type
{
    TANG_CHILD_STATEMENT,
    TANG_CHILD_BLOCK
};

struct Tang_Block_Template_Child
{
    Tang_Block_Template_Child_Type  type;
    Tang_Block_Template_Child_Data  data;
    
    Tang_Block_Template_Child()
    {
        // PASS
    }
    Tang_Block_Template_Child(Tang_Block_Template_Child &&src);
    ~Tang_Block_Template_Child();
};

struct Tang_Block_Template
{
    std::vector<Tang_Block_Template_Child>      children;
    std::map<std::string, Tang_Name>            name_map;
    std::map<int, Tang_Block_Template*>         imported_block_map;
    std::vector<Tang_Name*>                     indeterminate_list, 
                                                parameter_list,
                                                instances; // or fields
    Tang_Block_Template                        *parent;
    Tang_Name                                  *this_name;
    int                                         indentation,
                                                this_id,
                                                parent_id;
    bool                                        is_loop,
                                                is_compilable;
    // NOT FINISHED: expression templates, return type, compiled versions
    
    Tang_Block_Template_Child* new_child();
    
    Tang_Block_Template(Tang_Block_Template *parent_in = nullptr, 
                        int indentation_in = 0);
};

#endif
