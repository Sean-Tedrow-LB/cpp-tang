#ifndef __TANG_MODULE_RAW__
#define __TANG_MODULE_RAW__
#include "tang_comment_removal.hpp"
#include "ix_unicode.hpp"
#include <cstdlib>
#include <iostream>
#include "tang_number.hpp"


enum Tang_Read_Status
{
    TANG_READ_SUCCESS,
    TANG_READ_SKIPPED_MALFORMED,
    TANG_READ_FAILURE
};


struct Tang_Read_State
{
    const char *c_src;
    int c_len, line_number = 1;
    IX_Code_Point c;
    
    Tang_Read_Status read_character(Tang_File_Reader &source)
    {
        int len_left;
        Tang_Read_Status r = TANG_READ_SUCCESS;
        
        while(true)
        {
            len_left = source.length_left();
            if(!len_left)
            {
                return TANG_READ_FAILURE;
            }
            c_src = source.get();
            c = 0;
            c_len = ix_read_character(c_src, len_left, c);
            if(c_len > 0)
            {
                break;
            }
            Tang_Unit_Hex_String hex_string = tang_units_as_hex(c_src, -c_len);
            std::cout << "Malformed UTF-8 character encountered on line " <<
                         line_number << " consisting of units:";
            for(int i = 0; i < (int)hex_string.size(); i++)
            {
                Tang_Unit_Hex &u = hex_string[i];
                std::cout << " 0x" << u[0] << u[1];
            }
            std::cout << std::endl;
            source.advance(-c_len);
            r = TANG_READ_SKIPPED_MALFORMED;
        }
        
        
        
        if(c == '\n')
        {
            line_number++;
        }
        else if(c == '\r')
        {
            line_number++;
            c = '\n';
            c_src = "\n";
            
            source.advance(1);
            len_left = source.length_left();
            if(!len_left)
            {
                return r;
            }
            IX_Code_Point temp_c = 0;
            ix_read_character(source.get(), len_left, temp_c);
            if(temp_c == '\n')
            {
                source.advance(1);
            }
            return r;
        }
        source.advance(c_len);
        return r;
    }
    void append(std::string &out) const
    {
        for(int i = 0; i < c_len; i++)
        {
            out += c_src[i];
        }
    }
};


void Tang_Text_Without_Comments::from(Tang_File_Reader &source)
{
    Tang_Read_State state;
    while(state.read_character(source) != TANG_READ_FAILURE)
    {
        if(state.c == '/')
        {
            Tang_Read_Status s = state.read_character(source);
            if(s == TANG_READ_FAILURE)
            {
                return;
            }
            if((state.c != '*') || (s == TANG_READ_SKIPPED_MALFORMED))
            {
                text += '/';
            }
            else
            {
                if(state.read_character(source) == TANG_READ_FAILURE)
                {
                    return;
                }
                int comment_depth = 1;
                do
                {
                    if(state.c == '/')
                    {
                        s = state.read_character(source);
                        if(s == TANG_READ_FAILURE)
                        {
                            return;
                        }
                        if((state.c == '*') && 
                           (s != TANG_READ_SKIPPED_MALFORMED))
                        {
                            comment_depth++;
                        }
                    }
                    else if(state.c == '*')
                    {
                        s = state.read_character(source);
                        if(s == TANG_READ_FAILURE)
                        {
                            return;
                        }
                        if((state.c == '/') && 
                           (s != TANG_READ_SKIPPED_MALFORMED))
                        {
                            comment_depth--;
                        }
                    }
                    if(state.c == '\n')
                    {
                        text += '\n';
                    }
                    if(state.read_character(source) == TANG_READ_FAILURE)
                    {
                        return;
                    }
                }
                while(comment_depth);
            }
        }
        state.append(text);
    }
}


#endif
