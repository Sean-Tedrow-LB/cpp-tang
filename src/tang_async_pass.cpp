#include "tang_async_pass.hpp"
#include "tang_keyword_search.hpp"
#include <iostream>


// TODO: improve tabs warning: should list which lines the tabs were found on.



// NOTE: returns whether or not tabs were found
static bool statementify_module(Tang_Module_Node *module)
{
    bool tabs_found = false;
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
                return tabs_found;
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
                        return tabs_found;
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
                    if(c == '\t')
                    {
                        tabs_found = true;
                    }
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
                return tabs_found;
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
                        return tabs_found;
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
                    return tabs_found;
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
                            return tabs_found;
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

static void async_manage_last_statement(Tang_Last_Statement &last_statement,
                                        Tang_Block_Template *child_block,
                                        Tang_Block_Template **child_func_block)
{
    switch(last_statement.type)
    {
    case TANG_LAST_STATEMENT_FUNCTION:
        *child_func_block = child_block;
        last_statement.synonym.function->block = child_block;
        break;
    case TANG_LAST_STATEMENT_STRUCTURE:
        last_statement.synonym.structure->block = child_block;
    default:
        break;
    }
}


static bool async_statement_iterate(Tang_Keyword_Map &handler_map, 
                                    Tang_Module_Tracker &tracker,
                                    Tang_Module_Node *module,
                                    Tang_Block_Template *block, 
                                    Tang_Block_Template *func_block)
{
    Tang_Last_Statement last_statement;
    for(int i = 0; i < (int)block->children.size(); i++)
    {
        keyword_search<async_statement_iterate,
                       async_manage_last_statement>
                      (handler_map, tracker, module, block, 
                       func_block, last_statement, i);
    }
    return true;
}



void module_async_pass(Tang_Module_Tracker &tracker, 
                       Tang_Module_Node *module,
                       Tang_Keyword_Map async_keyword_handlers)
{
    bool tabs_found = statementify_module(module);
    if(tabs_found)
    {
        std::cout << "WARNING: tab characters found in module \"" <<
                     module->get_module_path() << 
                     "\".  (Tabs aren't interpreted as proper whitespace "
                     "for the purposes of forming blocks)" << std::endl;
    }
    async_statement_iterate(async_keyword_handlers, tracker, module, 
                            module->root_block, nullptr);
}

static bool module_keyword_handler(Tang_Module_Tracker &tracker, 
                                     Tang_Module_Node *module,
                                     Tang_Block_Template *block,
                                     int statement_i, 
                                     Tang_Last_Statement &statement_out)
{
    // TODO
    return false;
}

static bool library_keyword_handler(Tang_Module_Tracker &tracker, 
                                    Tang_Module_Node *module,
                                    Tang_Block_Template *block,
                                    int statement_i, 
                                    Tang_Last_Statement &statement_out)
{
    // TODO
    return false;
}

static bool const_keyword_handler(Tang_Module_Tracker &tracker, 
                                  Tang_Module_Node *module,
                                  Tang_Block_Template *block,
                                  int statement_i, 
                                  Tang_Last_Statement &statement_out)
{
    // TODO
    return false;
}

static bool class_keyword_handler(Tang_Module_Tracker &tracker, 
                                  Tang_Module_Node *module,
                                  Tang_Block_Template *block,
                                  int statement_i, 
                                  Tang_Last_Statement &statement_out)
{
    // TODO
    return false;
}

static bool form_keyword_handler(Tang_Module_Tracker &tracker, 
                                 Tang_Module_Node *module,
                                 Tang_Block_Template *block,
                                 int statement_i, 
                                 Tang_Last_Statement &statement_out)
{
    // TODO
    return false;
}

static bool alias_keyword_handler(Tang_Module_Tracker &tracker, 
                                  Tang_Module_Node *module,
                                  Tang_Block_Template *block,
                                  int statement_i, 
                                  Tang_Last_Statement &statement_out)
{
    // TODO
    return false;
}

static bool func_keyword_handler(Tang_Module_Tracker &tracker, 
                                 Tang_Module_Node *module,
                                 Tang_Block_Template *block,
                                 int statement_i, 
                                 Tang_Last_Statement &statement_out)
{
    // TODO
    return false;
}


void tang_async_pass(Tang_Module_Tracker &tracker)
{
    Tang_Keyword_Map keyword_map;
    keyword_map["module"]   = module_keyword_handler;
    keyword_map["library"]  = library_keyword_handler;
    keyword_map["const"]    = const_keyword_handler;
    keyword_map["class"]    = class_keyword_handler;
    keyword_map["form"]     = form_keyword_handler;
    keyword_map["alias"]    = alias_keyword_handler;
    keyword_map["func"]     = func_keyword_handler;
    for(int i = 0; i < (int)tracker.modules.size(); i++)
    {
        module_async_pass(tracker, tracker.modules[i], keyword_map);
    }
}

