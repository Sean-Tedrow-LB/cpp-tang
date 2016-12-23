#ifndef __TANG_KEYWORD_SEARCH__
#define __TANG_KEYWORD_SEARCH__

#include <cctype>
#include <map>
#include "tang_modules.hpp"
#include "tang_block_template.hpp"
#include "tang_name.hpp"


enum Tang_Last_Statement_Type
{
    TANG_LAST_STATEMENT_UNSET,
    TANG_LAST_STATEMENT_FUNCTION,
    TANG_LAST_STATEMENT_STRUCTURE
};

union Tang_Last_Statement_Union
{
    Tang_Name_Function_Synonym *function;
    Tang_Name_Structure_Synonym *structure;
};

struct Tang_Last_Statement
{
    Tang_Last_Statement_Type type;
    Tang_Last_Statement_Union synonym;
    
    void reset()
    {
        type = TANG_LAST_STATEMENT_UNSET;
    }
};

// return false if you want the function to be marked uncompilable
typedef bool (*Tang_Keyword_Handler)(Tang_Module_Tracker &tracker, 
                                     Tang_Module_Node *module,
                                     Tang_Block_Template *block,
                                     int statement_i, 
                                     Tang_Last_Statement &statement_out);


typedef std::map<std::string, Tang_Keyword_Handler> Tang_Keyword_Map;


// NOTE: returns false if compilation occurs.


template <bool keyword_iterate(Tang_Keyword_Map &handler_map, 
                               Tang_Module_Tracker &tracker,
                               Tang_Module_Node *module,
                               Tang_Block_Template *block, 
                               Tang_Block_Template *func_block),
          void manage_last_statement(Tang_Last_Statement &last_statement,
                                     Tang_Block_Template *child_block,
                                     Tang_Block_Template **child_func_block)>
bool keyword_search(Tang_Keyword_Map &handler_map, 
                    Tang_Module_Tracker &tracker,
                    Tang_Module_Node *module,
                    Tang_Block_Template *block, 
                    Tang_Block_Template *func_block,
                    Tang_Last_Statement &last_statement,
                    int i)
{
    Tang_Block_Template_Child &child = block->children[i];
    if(child.type == TANG_CHILD_STATEMENT)
    {
        last_statement.reset();
        std::string token;
        const char *s = child.data.statement.start;
        int j = child.data.statement.length;
        std::string out_string;
        for(int k = 0; k < j; k++)
        {
            char c = s[k];
            if(!std::isalpha((int)c))
            {
                break;
            }
            out_string += c;
        }
        auto handler_i = handler_map.find(token);
        if(handler_i != handler_map.end())
        {
            Tang_Keyword_Handler handler = handler_i->second;
            bool successful = handler(tracker, module, 
                                      block, i, last_statement);
            if(!successful && func_block)
            {
                func_block->is_compilable = false;
                return false;
            }
        }
    }
    else
    {
        Tang_Block_Template *child_block = child.data.block,
                            *child_func_block = func_block;
        manage_last_statement(last_statement, child_block, &child_func_block);
        bool successful = keyword_iterate(handler_map, tracker, module, 
                                          child_block, child_func_block);
        if(!successful)
        {
            return false;
        }
    }
    return true;
}

#endif
