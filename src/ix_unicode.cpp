#include "ix_unicode.hpp"

/*
conditions for finding a character invalid:
- an unexpected continuation unit
- a start unit not followed by enough continuation units
- a sequence that decodes to a value greater than 0x10FFFF
- a sequence that decodes to a utf16 surrogate code point (0xD800 through 0xDFFF)
- a null unit
*/

inline int ix_do_read_character(const char *source, int units_left, IX_Code_Point &code_point, 
                                char c, int length, char gatherer)
{
    if(units_left < length) // if there aren't enough units left to hold character
    {
        return -units_left;
    }
    IX_Code_Point cp = (IX_Code_Point)(c & gatherer); // gather initial unit
    for(int i = 1; i < length; i++) // gather the continuation units
    {
        c = source[i];
        if(!c) // unexpected null
        {
            return -i - 1;
        }
        if(!(c & (1 << 7) && !(c & (1 << 6)))) // unexpected non continuation unit
        {
            return -i;
        }
        cp <<= 6;
        cp |= c & 0x3F;
    }
    if(cp >= 0x110000 || (cp >= 0xD8000 && cp < 0xE000)) // if code point is invalid
    {
        return -length;
    }
    code_point = cp;
    return length;
}
template<>
int ix_read_character(const char *source, int units_left, IX_Code_Point &code_point)
{
    char c = source[0];
    if(c == '\0') // unexpected null
    {
        return -1;
    }
    else if(!(c & (1 << 7))) // if single-unit character
    {
        code_point = (IX_Code_Point)c;
        return 1;
    }
    else if(!(c & (1 << 6))) // if unexpected continuation unit
    {
        return -1;
    }
    else if(!(c & (1 << 5))) // find length
    {
        return ix_do_read_character(source, units_left, code_point, c, 2, 0x1F);
    }
    else if(!(c & (1 << 4)))
    {
        return ix_do_read_character(source, units_left, code_point, c, 3, 0xF);
    }
    else if(!(c & (1 << 3)))
    {
        return ix_do_read_character(source, units_left, code_point, c, 4, 0x7);
    }
    else // the initial unit says the character is > 4 units long, which is wrong
    {
        return -1;
    }
}
template<>
int ix_read_character(const char16_t *source, int units_left, IX_Code_Point &code_point)
{
    char16_t c = source[0];
    if(!c)
    {
        return -1;
    }
    else if(c < 0xD800 || c >= 0xE000)
    {
        code_point = (IX_Code_Point)c;
        return 1;
    }
    else if(c >= 0xDC00 || units_left < 2)
    {
        return -1;
    }
    else
    {
        IX_Code_Point cp = 0x10000;
        cp += (c - 0xD800) << 10;
        cp += source[1] - 0xDC00;
        if(cp >= 0x110000 || (cp >= 0xD8000 && cp < 0xE000))
        {
            return -2;
        }
        code_point = cp;
        return 2;
    }
}



template<typename UF, typename UT>
inline int ix_measure_string(const UF *source, int source_length)
{
    int to_length = 0;
    for(int i = 0; i < source_length;)
    {
        IX_Code_Point code_point = 0;
        int char_len = ix_read_character(source + i, source_length - i, code_point);
        if(char_len > 0)
        {
            to_length += ix_measure_code_point<UT>(code_point);
            i += char_len;
        }
        else
        {
            i -= char_len;
        }
    }
    return to_length;
}
int ix_measure_utf8_to_utf16(const char *source, int source_length)
{
    return ix_measure_string<char, char16_t>(source, source_length);
}
int ix_measure_utf16_to_utf8(const char16_t *source, int source_length)
{
    return ix_measure_string<char16_t, char>(source, source_length);
}




template<>
inline int ix_write_character(char *destination, IX_Code_Point code_point)
{
    int length = ix_measure_code_point<char>(code_point);
    if(length == 1)
    {
        destination[0] = (char)code_point;
    }
    else
    {
        for(int i = length - 1; i; i--)
        {
            destination[i] = (((char)code_point) & 0x3F) | (1 << 7);
            code_point >>= 6;
        }
        switch(length)
        {
        case 2:
            {
                destination[0] = ((char)code_point) | 0xC0;
            }
            break;
        case 3:
            {
                destination[0] = ((char)code_point) | 0xE0;
            }
            break;
        case 4:
            {
                destination[0] = ((char)code_point) | 0xF0;
            }
        default:
            break;
        }
    }
    return length;
}
template<>
inline int ix_write_character(char16_t *destination, IX_Code_Point code_point)
{
    if(code_point < 0x10000)
    {
        destination[0] = code_point;
        return 1;
    }
    else
    {
        code_point -= 0x10000;
        destination[0] = (code_point >> 10) + 0xD800;
        destination[1] = (code_point & 0x3FF) + 0xDC00;
        return 2;
    }
}



template<typename UF, typename UT>
inline void ix_convert_string(UT *destination, const UF *source, int source_length)
{
    for(int i = 0, j = 0; i < source_length;)
    {
        IX_Code_Point cp = 0;
        int src_char_len = ix_read_character(source + i, source_length - i, cp);
        if(src_char_len > 0)
        {
            j += ix_write_character(destination + j, cp);
            i += src_char_len;
        }
        else
        {
            i -= src_char_len;
        }
    }
}

void ix_convert_utf8_to_utf16(char16_t *destination, const char *source, int source_length)
{
    ix_convert_string(destination, source, source_length);
}
void ix_convert_utf16_to_utf8(char *destination, const char16_t *source, int source_length)
{
    ix_convert_string(destination, source, source_length);
}
