/*
// return false if you want the function to be marked uncompilable
typedef bool 
(*Tang_Keyword_Handler_Async)(Tang_Module_Tracker &tracker, 
                              Tang_Module_Node *module,
                              Tang_Block_Template *block,
                              Tang_Block_Template *out_block,
                              int statement_i, 
                              Tang_Last_Statement_Async &last_statement);


typedef std::map<std::string, Tang_Keyword_Handler_Async> Tang_Keyword_Map_Async;

// TODO: I think making functions non-compilable is unnecessary in this pass.

static bool block_async_pass(Tang_Module_Tracker &tracker, 
                             Tang_Module_Node *module, 
                             const Tang_Keyword_Map_Async &handler_map, 
                             Tang_Block_Template *block, 
                             Tang_Block_Template *out_block
                             Tang_Block_Template *func_block)
{
    Tang_Last_Statement_Async last_statement;
    for(int i = 0; i < (int)block->children.size(); i++)
    {
        Tang_Block_Template_Child &child = block->children[i];
        if(!child.block)
        {
            if(child.is_erroneous && func_block)
            {
                func_block->is_compilable = false;
                return false;
            }
            if(child.parts.empty())
            {
                continue;
            }
            if(child.parts[0] != TANG_PART_WORD)
            {
                continue;
            }
            auto handler_i = handler_map.find(part.value_string);
            if(handler_i == handler_map.end())
            {
                continue;
            }
            Tang_Keyword_Handler_Async handler = handler_i->second;
            bool successful = handler(tracker, module, block, 
                                      out_block, i, last_statement);
            if(!successful && func_block)
            {
                func_block->is_compilable = false;
                return false;
            }
        }
        else
        {
            Tang_Block_Template *child_block       = child.block,
                                *child_out_block   = child.block;
                                *child_func_block  = func_block;
            switch(last_statement.type)
            {
            case TANG_LAST_STATEMENT_FUNCTION:
                *child_func_block = child_block;
                last_statement.data.function->block = child_block;
                break;
            case TANG_LAST_STATEMENT_STRUCTURE:
                last_statement.data.structure->block = child_block;
                break;
            case TANG_LAST_STATEMENT_META:
                child_out_block = out_block;
            default:
                break;
            }
            bool successful = block_async_pass(tracker, module, handler_map,
                                               child_block, child_out_block, 
                                               child_func_block);
            if(!successful)
            {
                return false;
            }
        }
    }
    return true;
}
*/
