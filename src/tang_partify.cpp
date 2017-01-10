#include "tang_partify.hpp"
#include "ix_unicode.hpp"
#include "tang_number.hpp"
#include <iostream>




#define CHARACTER_ERROR   -1
#define CHARACTER_END     -2

template <void write_codepoint(IX_Code_Point cp, Tang_Part *part_out),
          int write_units(char first_unit, const char *units,
                          int length_left, Tang_Part *part_out)>
int read_character(const char *units, int length_left, char part_ender,
                   const char *part_type_string,
                   int &line_number, Tang_Part *part_out,
                   const Tang_Character_Compendium &compendium)
{
    char c = units[0];
    if(c == '\\')
    {
        if(length_left < 2)
        {
            std::cout << "Encountered end of statement before escape sequence "
                         "in " << part_type_string << " literal was "
                         "satisfied on line " << line_number << std::endl;
            return length_left;
        }
        c = units[1];
        if((c == 'x') || (c == 'X'))
        {
            for(int i = 2;; i++)
            {
                if(i >= length_left)
                {
                    std::cout << "Encountered end of statement before "
                                 "escape sequence in " << part_type_string << 
                                 " literal was satisfied on line " << 
                                 line_number << std::endl;
                    return i;
                }
                c = units[i];
                if(c == part_ender)
                {
                    std::cout << "Encountered end of " << part_type_string <<
                                 " literal before escape sequence was "
                                 "satisfied on line " << 
                                 line_number << std::endl;
                    return i;
                }
                else if((c == 'x') || (c == 'X'))
                {
                    IX_Code_Point cp = tang_read_hex(units + 2, i - 2);
                    write_codepoint(cp, part_out);
                    return i + 1;
                }
                else if(!compendium.lookup_number_literal(c))
                {
                    std::cout << "Encountered unexpected character in an "
                                 "escaped hex character on line " << 
                                 line_number << std::endl;
                    return CHARACTER_ERROR;
                }
            }
        }
        else if(c == '\n')
        {
            line_number++;
            std::cout << "Encountered end of line before escape sequence "
                         "in " << part_type_string << " literal was "
                         "satisfied on line " << line_number << std::endl;
            return 2;
        }
        else
        {
            c = compendium.lookup_char_escape(c);
            return write_units(c, units + 1, length_left - 1, part_out) + 1;
        }
    }
    else if(c == part_ender)
    {
        return CHARACTER_END;
    }
    else
    {
        return write_units(c, units, length_left, part_out);
    }
}


void write_codepoint_string(IX_Code_Point cp, Tang_Part *part_out)
{
    int cp_len = ix_measure_code_point<char>(cp);
    std::string &v_str = part_out->value_string;
    int old_len = (int)v_str.length();
    v_str.resize(old_len + cp_len);
    char *s = &(v_str[old_len]);
    ix_write_character(s, cp);
}

int write_units_string(char first_unit, const char *units,
                       int length_left, Tang_Part *part_out)
{
    part_out->value_string += first_unit;
    return 1;
}

void write_codepoint_character(IX_Code_Point cp, Tang_Part *part_out)
{
    part_out->code_point = cp;
}

#define MEASURE_CHAR(c)     ( \
                                !(c & (1 << 5)) ? 2 : \
                                ( \
                                    !(c & (1 << 4)) ? 3 : 4 \
                                ) \
                            )

int write_units_character(char first_unit, const char *units,
                           int length_left, Tang_Part *part_out)
{
    if(!(first_unit & (1 << 7))) // single unit
    {
        part_out->code_point = first_unit;
        return 1;
    }
    else
    {
        int len = ix_read_character(units, length_left, part_out->code_point);
        return len;
    }
}


static void partify_statement(Tang_Block_Template_Child &child,
                              const Tang_Character_Compendium &compendium)
{
    int line_number = child.line_number;
    const char *s = child.start;
    int i = 0, j = child.length;
    char c = ' ';
    while(true)
    {
        while(true)
        {
            if(i >= j)
            {
                return;
            }
            c = s[i];
            if(c == '\n')
            {
                line_number++;
                i++;
            }
            else
            {
                if((c != ' ') && (c != '\t') && (c != '\\'))
                {
                    break;
                }
                i++;
            }
        }
        Tang_Part *part = child.new_part();
        Tang_Part_Type type = (Tang_Part_Type)compendium.lookup_part_type(c);
        part->type = type;
        part->line_number = line_number;
        switch(type)
        {
        case TANG_PART_WORD:
            while(true)
            {
                char wc = compendium.lookup_word(c);
                if(!wc)
                {
                    break;
                }
                part->value_string += wc;
                i++;
                if(i >= j)
                {
                    return;
                }
                c = s[i];
            }
            break;
        case TANG_PART_NUMBER:
            do
            {
                part->value_string += c;
                i++;
                if(i >= j)
                {
                    return;
                }
                c = s[i];
            }
            while(compendium.lookup_number_literal(c) || (c == '.'));
            break;
        case TANG_PART_STRING:
            i++;
            while(true)
            {
                if(i >= j)
                {
                    std::cout << "Encountered end of statement before end of"
                                 " string literal was reached on line " <<
                                 line_number << std::endl;
                    return;
                }
                int read_len = read_character<write_codepoint_string,
                                              write_units_string>
                                             (s + i, j - i, '\"', "string",
                                              line_number, part, compendium);
                if(read_len == CHARACTER_END)
                {
                    i++;
                    break;
                }
                else if(read_len == CHARACTER_ERROR)
                {
                    child.is_erroneous = true;
                    return;
                }
                else
                {
                    i += read_len;
                }
            }
            break;
        case TANG_PART_CHARACTER:
            {
                i++;
                if(i >= j)
                {
                    std::cout << "Encountered end of statement before end of"
                                 " character literal was reached on line " <<
                                 line_number << std::endl;
                    return;
                }
                int read_len = read_character<write_codepoint_character,
                                              write_units_character>
                                             (s + i, j - i, '\'', "character",
                                              line_number, part, compendium);
                if(read_len == CHARACTER_END)
                {
                    std::cout << "Character literal on line " <<
                                 line_number << " was empty" << std::endl;
                    child.is_erroneous = true;
                    return;
                }
                else if(read_len == CHARACTER_ERROR)
                {
                    child.is_erroneous = true;
                    return;
                }
                else
                {
                    i += read_len;
                    if(i >= j)
                    {
                        std::cout << "Encountered end of statement before end "
                                     "of character literal was reached "
                                     "on line " << line_number << std::endl;
                    }
                    else
                    {
                        c = s[i];
                        if(c == '\'')
                        {
                            i++;
                        }
                        else
                        {
                            std::cout << "Character literal on line " <<
                                         line_number << " wasn't closed "
                                         "properly" << std::endl;
                        }
                    }
                }
            }
            break;
        case TANG_PART_PUNCTUATION:
            {
                std::string punc_string;
                do
                {
                    punc_string += c;
                    i++;
                    if(i >= j)
                    {
                        break;
                    }
                    c = s[i];
                }
                while(compendium.lookup_punctuation(c));
                int p_i = 0;
                while(true)
                {
                    int p_len = 0;
                    Tang_Part_Punctuation 
                    p = compendium.lookup_punctuation_type(punc_string, p_len);
                    if(p == TANG_P_UNRECOGNIZED)
                    {
                        std::cout << "Found an unrecognized punctuation mark \"" << 
                                     punc_string << "\" on line " << 
                                     line_number << std::endl;
                        child.is_erroneous = true;
                        return;
                    }
                    part->punctuation = p;
                    p_i += p_len;
                    if(p_i >= (int)punc_string.length())
                    {
                        break;
                    }
                    punc_string.erase(0, p_len);
                    part = child.new_part();
                    part->type = TANG_PART_PUNCTUATION;
                    part->line_number = line_number;
                }
            }
            break;
        case TANG_PART_ENCLOSER:
            part->code_point = c;
            i++;
            if(i >= j)
            {
                return;
            }
            break;
        case TANG_PART_ERRONEOUS:
            {
                Tang_Unit_Hex hex;
                tang_unit_as_hex(c, hex);
                std::cout << "Found a part that is of an unknown type on "
                             "line " << line_number << " that starts with "
                             "the UTF-8 unit 0x" << hex[0] << hex[1] << std::endl;
                child.is_erroneous = true;
            }
            return;
        default:
            break;
        }
    }
}

static void partify_block(Tang_Block_Template *block,
                          const Tang_Character_Compendium &compendium)
{
    for(int i = 0; i < (int)block->children.size(); i++)
    {
        Tang_Block_Template_Child &child = block->children[i];
        if(child.block)
        {
            partify_block(child.block, compendium);
        }
        else
        {
            partify_statement(child, compendium);
        }
    }
}

void partify_module(Tang_Module_Node *module, 
                    const Tang_Character_Compendium &compendium)
{
    partify_block(module->root_block, compendium);
}
