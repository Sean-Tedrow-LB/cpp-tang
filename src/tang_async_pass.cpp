#include "tang_async_pass.hpp"
#include <iostream>
#include <vector>
#include <map>
#include "tang_character_map.hpp"
#include "tang_statementify.hpp"
#include "tang_partify.hpp"





enum Tang_Last_Statement_Type_Async
{
    TANG_LAST_STATEMENT_UNSET_ASYNC,
    TANG_LAST_STATEMENT_FUNCTION,
    TANG_LAST_STATEMENT_STRUCTURE,
    TANG_LAST_STATEMENT_META_IF_ASYNC,
    TANG_LAST_STATEMENT_META_ELSE_ASYNC
};

union Tang_Last_Statement_Union_Async
{
    Tang_Name_Function_Synonym   *function;
    Tang_Name_Structure_Synonym  *structure;
    bool                          meta_if_result;
};

struct Tang_Last_Statement_Async
{
    Tang_Last_Statement_Type_Async type = TANG_LAST_STATEMENT_UNSET_ASYNC;
    Tang_Last_Statement_Union_Async data;
    
    void reset()
    {
        type = TANG_LAST_STATEMENT_UNSET_ASYNC;
    }
};

typedef void 
(*Tang_Keyword_Handler_Async)(Tang_Module_Tracker &tracker, 
                              Tang_Module_Node *module,
                              Tang_Block_Template *block,
                              Tang_Block_Template *out_block,
                              int statement_i, 
                              Tang_Last_Statement_Async &last_statement);


typedef std::map<std::string, Tang_Keyword_Handler_Async> Tang_Keyword_Map_Async;



static void block_async_pass(Tang_Module_Tracker &tracker, 
                             Tang_Module_Node *module, 
                             const Tang_Keyword_Map_Async &handler_map, 
                             Tang_Block_Template *block, 
                             Tang_Block_Template *out_block)
{
    Tang_Last_Statement_Async last_statement;
    for(int i = 0; i < (int)block->children.size(); i++)
    {
        Tang_Block_Template_Child &child = block->children[i];
        if(!child.block)
        {
            if(child.parts.empty() || child.is_erroneous)
            {
                last_statement.reset();
                continue;
            }
            Tang_Part &part = child.parts[0];
            if(part.type != TANG_PART_WORD)
            {
                last_statement.reset();
                continue;
            }
            auto handler_i = handler_map.find(part.value_string);
            if(handler_i == handler_map.end())
            {
                last_statement.reset();
                continue;
            }
            Tang_Keyword_Handler_Async handler = handler_i->second;
            handler(tracker, module, block, out_block, i, last_statement);
        }
        else
        {
            Tang_Block_Template *child_block       = child.block,
                                *child_out_block   = child.block;
            switch(last_statement.type)
            {
            case TANG_LAST_STATEMENT_FUNCTION:
                last_statement.data.function->block = child_block;
                break;
            case TANG_LAST_STATEMENT_STRUCTURE:
                last_statement.data.structure->block = child_block;
                break;
            case TANG_LAST_STATEMENT_META_IF_ASYNC:
            case TANG_LAST_STATEMENT_META_ELSE_ASYNC:
                child_out_block = out_block;
            default:
                break;
            }
            block_async_pass(tracker, module, handler_map, 
                             child_block, child_out_block);
        }
    }
}

static void module_async_pass(Tang_Module_Tracker &tracker, 
                              Tang_Module_Node *module,
                              const Tang_Keyword_Map_Async &async_keyword_handlers,
                              const Tang_Character_Compendium &compendium)
{
    std::cout << "\n\n\n*** Performing asynchronous pass on module \"" <<
                 module->get_module_path() << "\" ***" << std::endl;
    std::vector<int> tabs;
    statementify_module(module, tabs);
    partify_module(module, compendium);
    if(!tabs.empty())
    {
        std::cout << "WARNING: tab characters found in module \"" <<
                     module->get_module_path() << 
                     "\".  (Tabs aren't interpreted as proper whitespace "
                     "in the process of forming blocks.)  "
                     "Tabs were found on line(s): " << std::endl;
        for(int i = 0; i < (int)tabs.size(); i++)
        {
            std::cout << tabs[i] << std::endl;
        }
    }
    Tang_Block_Template *block = module->root_block;
    block_async_pass(tracker, module, async_keyword_handlers, block, block);
}

static void module_keyword_handler(Tang_Module_Tracker &tracker, 
                                   Tang_Module_Node *module,
                                   Tang_Block_Template *block,
                                   Tang_Block_Template *out_block,
                                   int statement_i, 
                                   Tang_Last_Statement_Async &last_statement)
{
    std::cout << "module keyword" << std::endl;
    // TODO
}

static void library_keyword_handler(Tang_Module_Tracker &tracker, 
                                    Tang_Module_Node *module,
                                    Tang_Block_Template *block,
                                    Tang_Block_Template *out_block,
                                    int statement_i, 
                                    Tang_Last_Statement_Async &last_statement)
{
    std::cout << "library keyword" << std::endl;
    // TODO
}

static void const_keyword_handler(Tang_Module_Tracker &tracker, 
                                  Tang_Module_Node *module,
                                  Tang_Block_Template *block,
                                  Tang_Block_Template *out_block,
                                  int statement_i, 
                                  Tang_Last_Statement_Async &last_statement)
{
    std::cout << "const keyword" << std::endl;
    // TODO
}

static void class_keyword_handler(Tang_Module_Tracker &tracker, 
                                  Tang_Module_Node *module,
                                  Tang_Block_Template *block,
                                  Tang_Block_Template *out_block,
                                  int statement_i, 
                                  Tang_Last_Statement_Async &last_statement)
{
    std::cout << "class keyword" << std::endl;
    // TODO
}

static void form_keyword_handler(Tang_Module_Tracker &tracker, 
                                 Tang_Module_Node *module,
                                 Tang_Block_Template *block,
                                 Tang_Block_Template *out_block,
                                 int statement_i, 
                                 Tang_Last_Statement_Async &last_statement)
{
    std::cout << "form keyword" << std::endl;
    // TODO
}

static void union_keyword_handler(Tang_Module_Tracker &tracker, 
                                  Tang_Module_Node *module,
                                  Tang_Block_Template *block,
                                  Tang_Block_Template *out_block,
                                  int statement_i, 
                                  Tang_Last_Statement_Async &last_statement)
{
    std::cout << "union keyword" << std::endl;
    // TODO
}

static void alias_keyword_handler(Tang_Module_Tracker &tracker, 
                                  Tang_Module_Node *module,
                                  Tang_Block_Template *block,
                                  Tang_Block_Template *out_block,
                                  int statement_i, 
                                  Tang_Last_Statement_Async &last_statement)
{
    std::cout << "alias keyword" << std::endl;
    // TODO
}

static void func_keyword_handler(Tang_Module_Tracker &tracker, 
                                 Tang_Module_Node *module,
                                 Tang_Block_Template *block,
                                 Tang_Block_Template *out_block,
                                 int statement_i, 
                                 Tang_Last_Statement_Async &last_statement)
{
    std::cout << "func keyword" << std::endl;
    // TODO
}

static void meta_keyword_handler(Tang_Module_Tracker &tracker, 
                                 Tang_Module_Node *module,
                                 Tang_Block_Template *block,
                                 Tang_Block_Template *out_block,
                                 int statement_i, 
                                 Tang_Last_Statement_Async &last_statement)
{
    std::cout << "meta keyword" << std::endl;
    // TODO
}

static void enum_keyword_handler(Tang_Module_Tracker &tracker, 
                                 Tang_Module_Node *module,
                                 Tang_Block_Template *block,
                                 Tang_Block_Template *out_block,
                                 int statement_i, 
                                 Tang_Last_Statement_Async &last_statement)
{
    std::cout << "enum keyword" << std::endl;
    // TODO
}

static void tang_async_keyword_map(Tang_Keyword_Map_Async &keyword_map)
{
    keyword_map["module"]   = module_keyword_handler;
    keyword_map["library"]  = library_keyword_handler;
    keyword_map["const"]    = const_keyword_handler;
    keyword_map["class"]    = class_keyword_handler;
    keyword_map["form"]     = form_keyword_handler;
    keyword_map["union"]    = union_keyword_handler;
    keyword_map["alias"]    = alias_keyword_handler;
    keyword_map["func"]     = func_keyword_handler;
    keyword_map["meta"]     = meta_keyword_handler;
    keyword_map["enum"]     = enum_keyword_handler;
}

void tang_async_pass(Tang_Module_Tracker &tracker)
{
    Tang_Character_Compendium compendium;
    Tang_Keyword_Map_Async keyword_map;
    tang_async_keyword_map(keyword_map);
    for(int i = 0; i < (int)tracker.modules.size(); i++)
    {
        module_async_pass(tracker, tracker.modules[i], 
                          keyword_map, compendium);
    }
}

