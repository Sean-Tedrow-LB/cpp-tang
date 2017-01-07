#include "tang_statementify.hpp"
#include "tang_block_template.hpp"
#include <string>

void statementify_module(Tang_Module_Node *module, 
                         std::vector<int> &tabs_out)
{
    Tang_Block_Template *block = module->root_block;
    const std::string &text = module->text.text;
    
    int i = 0;
    char c;
    int line_number = 1;
    while(true)
    {
        int indentation = 0;
        while(true)
        {
            if(i >= (int)text.length())
            {
                return;
            }
            c = text[i];
            if(c == '\n')
            {
                indentation = 0;
                line_number++;
            }
            else
            {
                if(c != ' ')
                {
                    if(c == '\t')
                    {
                        tabs_out.push_back(line_number);
                    }
                    break;
                }
                indentation++;
            }
            i++;
        }
        Tang_Block_Template_Child *child = nullptr;
        if(indentation > block->indentation)
        {
            child = block->new_child();
            block = new Tang_Block_Template(block, indentation);
            child->block = block;
            child = block->new_child();
        }
        else if(indentation < block->indentation)
        {
            do
            {
                block = block->parent;
            }
            while(indentation < block->indentation);
            child = block->new_child();
            if(indentation > block->indentation)
            {
                block = new Tang_Block_Template(block, indentation);
                child->block = block;
                child = block->new_child();
            }
        }
        else
        {
            child = block->new_child();
        }
        child->line_number = line_number;
        const char *start = &(text[i]);
        child->start = start;
        int j = 0;
        while(true)
        {
            if(i >= (int)text.length())
            {
                child->length = j;
                return;
            }
            c = text[i];
            if(c == '\n')
            {
                line_number++;
                i++;
                child->length = j;
                break;
            }
            else if(c == '\\')
            {
                i++; j++;
                if(i >= (int)text.length())
                {
                    child->length = j;
                    return;
                }
                c = text[i];
                if(c == '\n')
                {
                    line_number++;
                    i++; j++;
                }
            }
            else
            {
                i++; j++;
            }
        }
    }
}
