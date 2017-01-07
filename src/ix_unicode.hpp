
#ifndef __IX_UNICODE__
#define __IX_UNICODE__


/*
 * Provides functions for converting between UTF-8 and UTF-16.
 * 
 * unicode distinguishes between "characters" (the things that show up on 
 * the screen when a string is rendered) and what it refers to as "units" of data 
 * which make up each character (a single character can be made up of a sequence of units).
 * 
 * These functions do not handle null terminators at all.
 * 
 * Any invalid UTF units will just be silently removed during the conversion.
 * 
 * 
 * 
 * FUNCTIONS:
 * 
 * ix_measure_utf8_to_utf16(source, source_length),
 * ix_measure_utf16_to_utf8(source, source_length):
 *     Returns the number of units needed to store the source string after
 *     conversion.
 * 
 * ix_convert_utf8_to_utf16(destination, source, source_length),
 * ix_convert_utf16_to_utf8(destination, source, source_length):
 *     Once you've allocated the destination, use this function to convert the 
 *     source string, and store the result in the destination.
 * 
 * 
 * 
 * BYTE ORDER MARKS:
 * 
 * UTF-8 encoded text files might begin with a byte-order-mark which indicates
 * that it is a UTF-8 encoded text file. You will have to check for it when
 * you open a text file and remove it if it's there.
 * 
 * Two constants are provided to help with this process:
 * 
 * IX_UTF8_BOM_SIZE is the number of bytes/units in the UTF-8 BOM.
 * 
 * IX_UTF8_BOM is a constant array of UTF-8 units.  You can identify a UTF-8 BOM
 * by comparing the first IX_UTF8_BOM_SIZE bytes of a file to the elements of
 * IX_UTF8_BOM.
 */



#include <cwchar>
#include <cstddef>

const char IX_UTF8_BOM[3] = {(char)0xEF, (char)0xBB, (char)0xBF};
#define IX_UTF8_BOM_SIZE   3

int ix_measure_utf8_to_utf16(const char *source, int source_length);
void ix_convert_utf8_to_utf16(char16_t *destination, const char *source, int source_length);

int ix_measure_utf16_to_utf8(const char16_t *source, int source_length);
void ix_convert_utf16_to_utf8(char *destination, const char16_t *source, int source_length);





/*
 * ADVANCED FUNCTIONS:
 * 
 * An IX_Code_Point is a variable that describes a single character.
 * 
 * ix_measure_code_point<Unit>(code_point):
 *     Measure the number of Units that will be needed to store the character
 *     described by the given code point.  Unit can be char or char16_t
 * 
 * ix_read_character(source, number_of_units_left, code_point_return):
 *     Read the first character from the source and convert it to an IX_Code_Point.
 *     Will return the length in units of that character.  Returns a negative number 
 *     if invalid UTF was encountered (negate the number to get the length of the 
 *     invalid UTF).  source can be char* or char16_t*.
 * 
 * ix_write_character(destination, code_point):
 *     Write the character described by the given code point to the destination.
 *     destination can be char* or char16_t*.
 */

typedef int IX_Code_Point;

#define IX_CODE_POINT_IS_INVALID(cp) \
    ((cp >= 0x110000) || ((cp >= 0xD8000) && (cp < 0xE000)))

template<typename UT>
int ix_measure_code_point(IX_Code_Point code_point);
template<>
inline int ix_measure_code_point<char>(IX_Code_Point code_point)
{
    return code_point < 0x80 ? 1 : (code_point < 0x800 ? 2 : (code_point < 0x10000 ? 3 : 4));
}
template<>
inline int ix_measure_code_point<char16_t>(IX_Code_Point code_point)
{
    return code_point < 0x10000 ? 1 : 2;
}



template<typename U>
int ix_read_character(const U *source, int units_left, IX_Code_Point &code_point);

/*
template<>
int ix_read_character(const char *source, int units_left, IX_Code_Point &code_point);
template<>
int ix_read_character(const char16_t *source, int units_left, IX_Code_Point &code_point);*/



template<typename U>
int ix_write_character(U *destination, IX_Code_Point code_point);

/*
template<>
int ix_write_character(char *destination, IX_Code_Point code_point);
template<>
int ix_write_character(char16_t *destination, IX_Code_Point code_point);*/






















// COPIED AND PASTED INTO THIS HEADER TO MAKE SHIT WORK, GRUMBLE GRUMBLE

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
    if(IX_CODE_POINT_IS_INVALID(cp))
    {
        return -length;
    }
    code_point = cp;
    return length;
}
template<>
inline int ix_read_character(const char *source, int units_left, 
                             IX_Code_Point &code_point)
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
inline int ix_read_character(const char16_t *source, int units_left, IX_Code_Point &code_point)
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






#endif
