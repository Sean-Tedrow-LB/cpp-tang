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


Tang_Part* Tang_Block_Template_Child::new_part()
{
    int sz = (int)parts.size();
    parts.resize(sz + 1);
    return &(parts[sz]);
}

Tang_Block_Template_Child::~Tang_Block_Template_Child()
{
    delete block;
}

Tang_Block_Template_Child::Tang_Block_Template_Child(Tang_Block_Template_Child &&src)
{
    parts        = (std::vector<Tang_Part>&&)src.parts;
    block        = src.block;
    src.block    = nullptr;
    start        = src.start;
    length       = src.length;
    line_number  = src.line_number;
}
