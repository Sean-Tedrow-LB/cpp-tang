#include "tang_output.hpp"
#include <string>
#include <map>


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

static void output_block_statements(std::string &statement, 
                                    const Tang_Block_Template *block,
                                    Tang_File_Writer &writer)
{
    statement.clear();
    statement.resize(block->indentation, ' ');
    statement += std::string("BLOCK ") + std::to_string(block->this_id) + 
                 ", INDENTATION " + std::to_string(block->indentation) + ":\n";
    writer.write(statement);
    for(int i = 0; i < (int)block->children.size(); i++)
    {
        const Tang_Block_Template_Child &child = block->children[i];
        if(child.block)
        {
            output_block_statements(statement, child.block, writer);
        }
        else
        {
            statement.clear();
            statement.resize(block->indentation, ' ');
            statement += std::string("Line ") + 
                       std::to_string(child.line_number);
            if(child.is_erroneous)
            {
                statement += " (CONTAINS ERRORS): ";
            }
            else
            {
                statement += ": ";
            }
            int header_length = (int)statement.length();
            int statement_length = child.length;
            statement.resize(header_length + statement_length);
            char *out = &(statement[header_length]);
            const char *statement_in = child.start;
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
        output_block_statements(statement, module->root_block, writer);
    }
}


static void output_code_point(std::string &statement, IX_Code_Point cp)
{
    int old_length = (int)statement.length();
    int cp_length = ix_measure_code_point<char>(cp);
    statement.resize(old_length + cp_length);
    ix_write_character(&(statement[old_length]), cp);
}

typedef std::map<Tang_Part_Punctuation, std::string> Tang_Punctuation_Map;

#define ADD_TO_PUNC_MAP(map, THING) map[TANG_P_##THING] = TANG_PS_##THING

static void setup_punctuation_map(Tang_Punctuation_Map &map)
{
    ADD_TO_PUNC_MAP(map, ASSIGN);
    ADD_TO_PUNC_MAP(map, POINT);
    ADD_TO_PUNC_MAP(map, DPOINT);
    ADD_TO_PUNC_MAP(map, ADDRESS);
    ADD_TO_PUNC_MAP(map, ACCESS);
    ADD_TO_PUNC_MAP(map, GACCESS);
    ADD_TO_PUNC_MAP(map, DO);
    ADD_TO_PUNC_MAP(map, COMMA);
    ADD_TO_PUNC_MAP(map, EQ);
    ADD_TO_PUNC_MAP(map, NEQ);
    ADD_TO_PUNC_MAP(map, GT);
    ADD_TO_PUNC_MAP(map, LT);
    ADD_TO_PUNC_MAP(map, GTEQ);
    ADD_TO_PUNC_MAP(map, LTEQ);
    ADD_TO_PUNC_MAP(map, NOT);
    ADD_TO_PUNC_MAP(map, LAND);
    ADD_TO_PUNC_MAP(map, LOR);
    ADD_TO_PUNC_MAP(map, BAND);
    ADD_TO_PUNC_MAP(map, BOR);
    ADD_TO_PUNC_MAP(map, BXOR);
    ADD_TO_PUNC_MAP(map, SHR);
    ADD_TO_PUNC_MAP(map, SHL);
    ADD_TO_PUNC_MAP(map, MOD);
    ADD_TO_PUNC_MAP(map, MULT);
    ADD_TO_PUNC_MAP(map, DIV);
    ADD_TO_PUNC_MAP(map, ADD);
    ADD_TO_PUNC_MAP(map, SUB);
    ADD_TO_PUNC_MAP(map, BANDASSIGN);
    ADD_TO_PUNC_MAP(map, BORASSIGN);
    ADD_TO_PUNC_MAP(map, BXORASSIGN);
    ADD_TO_PUNC_MAP(map, SHRASSIGN);
    ADD_TO_PUNC_MAP(map, SHLASSIGN);
    ADD_TO_PUNC_MAP(map, MODASSIGN);
    ADD_TO_PUNC_MAP(map, MULTASSIGN);
    ADD_TO_PUNC_MAP(map, DIVASSIGN);
    ADD_TO_PUNC_MAP(map, ADDASSIGN);
    ADD_TO_PUNC_MAP(map, SUBASSIGN);
    ADD_TO_PUNC_MAP(map, QM);
}

static void output_block_parts(std::string &statement, 
                               const Tang_Block_Template *block,
                               Tang_File_Writer &writer,
                               Tang_Punctuation_Map &map)
{
    statement.clear();
    statement.resize(block->indentation, ' ');
    statement += std::string("BLOCK ") + std::to_string(block->this_id) + 
                 ", INDENTATION " + std::to_string(block->indentation) + ":\n";
    writer.write(statement);
    for(int i = 0; i < (int)block->children.size(); i++)
    {
        const Tang_Block_Template_Child &child = block->children[i];
        if(child.block)
        {
            output_block_parts(statement, child.block, writer, map);
        }
        else
        {
            statement.clear();
            statement.resize(block->indentation, ' ');
            statement += std::string("Line ") + 
                       std::to_string(child.line_number);
            if(child.is_erroneous)
            {
                statement += " CONTAINS ERRORS\n";
            }
            else
            {
                statement += ":";
                for(int j = 0; j < (int)child.parts.size(); j++)
                {
                    const Tang_Part &part = child.parts[j];
                    switch(part.type)
                    {
                    case TANG_PART_WORD:
                        statement += std::string(" word(") + 
                                     part.value_string + ")";
                        break;
                    case TANG_PART_NUMBER:
                        statement += std::string(" number(") +
                                     part.value_string + ")";
                        break;
                    case TANG_PART_STRING:
                        statement += std::string(" string(") +
                                     part.value_string + ")";
                        break;
                    case TANG_PART_CHARACTER:
                        statement += " character(";
                        output_code_point(statement, part.code_point);
                        statement += ")";
                        break;
                    case TANG_PART_PUNCTUATION:
                        statement += std::string(" punctuation(") +
                                     map[part.punctuation] + ")";
                        break;
                    case TANG_PART_ENCLOSER:
                        statement += " encloser(";
                        output_code_point(statement, part.code_point);
                        statement += ")";
                        break;
                    default:
                        break;
                    }
                }
                statement += '\n';
            }
            
            writer.write(statement);
        }
    }
}

void output_parts(const Tang_Module_Tracker &module_tracker,
                  Tang_File_Writer &writer)
{
    std::string statement;
    Tang_Punctuation_Map map;
    setup_punctuation_map(map);
    for(int i = 0; i < (int)module_tracker.modules.size(); i++)
    {
        Tang_Module_Node *module = module_tracker.modules[i];
        statement = std::string("\n\n\n***** FOLLOWING IS TEXT OF MODULE \"") + 
                              module->get_module_path() + "\" *****\n\n\n";
        writer.write(statement);
        output_block_parts(statement, module->root_block, writer, map);
    }
}
