#include "tang_output.hpp"
#include <string>


void output_text_without_comments(const Tang_Module_Tracker &module_tracker,
                                  Tang_File_Writer &writer)
{
    std::string header;
    for(int i = 0; i < (int)module_tracker.modules.size(); i++)
    {
        Tang_Module_Node *module = module_tracker.modules[i];
        header = std::string("\n\n\n***** FOLLOWING IS TEXT OF MODULE \"") + 
                                module->get_module_path() + "\" *****\n\n\n";
        writer.write(header);
        writer.write(module->text.text);
    }
}

static void output_block(std::string &statement, 
                         const Tang_Block_Template *block,
                         Tang_File_Writer &writer)
{
    statement.resize(block->indentation);
    for(int i = 0; i < block->indentation; i++)
    {
        statement[i] = ' ';
    }
    statement += std::string("BLOCK ") + std::to_string(block->this_id) + 
                 ", INDENTATION " + std::to_string(block->indentation) + ":\n";
    writer.write(statement);
    for(int i = 0; i < (int)block->children.size(); i++)
    {
        const Tang_Block_Template_Child &child = block->children[i];
        if(child.type == TANG_CHILD_BLOCK)
        {
            output_block(statement, child.data.block, writer);
        }
        else
        {
            statement.resize(block->indentation);
            for(int j = 0; j < block->indentation; j++)
            {
                statement[j] = ' ';
            }
            statement += std::string("Line ") + 
                       std::to_string(child.data.statement.line_number) + ": ";
            int header_length = (int)statement.length();
            int statement_length = child.data.statement.length;
            statement.resize(header_length + statement_length);
            char *out = &(statement[header_length]);
            const char *statement_in = child.data.statement.start;
            for(int j = 0; j < statement_length; j++)
            {
                out[j] = statement_in[j];
            }
            statement += '\n';
            writer.write(statement);
        }
    }
}

void output_blocks_and_statements(const Tang_Module_Tracker &module_tracker,
                                  Tang_File_Writer &writer)
{
    std::string statement;
    for(int i = 0; i < (int)module_tracker.modules.size(); i++)
    {
        Tang_Module_Node *module = module_tracker.modules[i];
        statement = std::string("\n\n\n***** FOLLOWING IS TEXT OF MODULE \"") + 
                              module->get_module_path() + "\" *****\n\n\n";
        writer.write(statement);
        output_block(statement, module->root_block, writer);
    }
}
