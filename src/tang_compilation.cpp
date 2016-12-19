#include "tang_compilation.hpp"
#include "tang_name.hpp"

struct Tang_Compiler_Data
{
    std::vector<Tang_Name_Function_Synonym*>  c_functions, 
                                              run_function_candidates;
    Tang_Block_Template                      *run_function;
    bool                                      too_many_run_functions;
    
    void module_async_pass(Tang_Module_Tracker &tracker, 
                           Tang_Module_Node *module);
    void async_pass(Tang_Module_Tracker &tracker);
    void sync_pass(Tang_Module_Tracker &tracker);
    bool compile_pass(/* ??? */);
};

static void statementify_module(Tang_Module_Node *module)
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
            if((c == '\r') || (c == '\n'))
            {
                indentation = 0;
                line_number++;
                i++;
                if(c == '\r')
                {
                    if(i >= (int)text.length())
                    {
                        return;
                    }
                    char n = text[i];
                    if(n == '\n')
                    {
                        i++;
                    }
                }
            }
            else
            {
                if(c != ' ')
                {
                    break;
                }
                indentation++;
                i++;
            }
        }
        Tang_Block_Template_Child *child = nullptr;
        if(indentation > block->indentation)
        {
            child = block->new_child();
            child->type = TANG_CHILD_BLOCK;
            block = new Tang_Block_Template(block, indentation);
            child->data.block = block;
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
                child->type = TANG_CHILD_BLOCK;
                block = new Tang_Block_Template(block, indentation);
                child->data.block = block;
                child = block->new_child();
            }
        }
        else
        {
            child = block->new_child();
        }
        child->type = TANG_CHILD_STATEMENT;
        child->data.statement.line_number = line_number;
        const char *start = &(text[i]);
        child->data.statement.start = start;
        int j = 0;
        while(true)
        {
            if(i >= (int)text.length())
            {
                child->data.statement.length = j;
                return;
            }
            c = text[i];
            if((c == '\r') || (c == '\n'))
            {
                line_number++;
                i++;
                if(c == '\r')
                {
                    if(i >= (int)text.length())
                    {
                        child->data.statement.length = j;
                        return;
                    }
                    char n = text[i];
                    if(n == '\n')
                    {
                        i++;
                    }
                }
                child->data.statement.length = j;
                break;
            }
            else if(c == '\\')
            {
                i++; j++;
                if(i >= (int)text.length())
                {
                    child->data.statement.length = j;
                    return;
                }
                c = text[i];
                if((c == '\r') || (c == '\n'))
                {
                    line_number++;
                    i++; j++;
                    if(c == '\r')
                    {
                        if(i >= (int)text.length())
                        {
                            child->data.statement.length = j;
                            return;
                        }
                        char n = text[i];
                        if(n == '\n')
                        {
                            i++; j++;
                        }
                    }
                }
            }
            else
            {
                i++; j++;
            }
        }
    }
}

void Tang_Compiler_Data::module_async_pass(Tang_Module_Tracker &tracker, 
                                           Tang_Module_Node *module)
{
    statementify_module(module);
    // TODO
}
void Tang_Compiler_Data::async_pass(Tang_Module_Tracker &tracker)
{
    for(int i = 0; i < (int)tracker.modules.size(); i++)
    {
        module_async_pass(tracker, tracker.modules[i]);
    }
}
void Tang_Compiler_Data::sync_pass(Tang_Module_Tracker &tracker)
{
    // TODO
}
bool Tang_Compiler_Data::compile_pass(/* ??? */)
{
    // TODO
    return false;
}


bool tang_compile(Tang_Module_Tracker &module_tracker)
{
    Tang_Compiler_Data compiler_data;
    compiler_data.async_pass(module_tracker);
    compiler_data.sync_pass(module_tracker);
    return compiler_data.compile_pass(/* ??? */);
}
