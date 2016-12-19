#include "tang_block_template.hpp"

static int next_id;

Tang_Block_Template::Tang_Block_Template(Tang_Block_Template *parent_in, 
                                         int indentation_in)
{
    parent = parent_in;
    this_name = nullptr;
    indentation = indentation_in;
    this_id = next_id++;
    is_loop = false;
    is_compilable = true;
    if(parent_in)
    {
        parent_id = parent_in->this_id;
    }
}

Tang_Block_Template_Child* Tang_Block_Template::new_child()
{
    int sz = (int)children.size();
    children.resize(sz + 1);
    return &(children[sz]);
}

Tang_Block_Template_Child::~Tang_Block_Template_Child()
{
    if(type == TANG_CHILD_BLOCK)
    {
        delete data.block;
    }
}

Tang_Block_Template_Child::Tang_Block_Template_Child(Tang_Block_Template_Child &&src)
{
    if(src.type == TANG_CHILD_BLOCK)
    {
        type = TANG_CHILD_BLOCK;
        data.block = src.data.block;
        src.data.block = nullptr;
    }
    else
    {
        type = TANG_CHILD_STATEMENT;
        data = src.data;
    }
}
