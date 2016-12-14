
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
template<>
int ix_read_character(const char *source, int units_left, IX_Code_Point &code_point);
template<>
int ix_read_character(const char16_t *source, int units_left, IX_Code_Point &code_point);



template<typename U>
int ix_write_character(U *destination, IX_Code_Point code_point);
template<>
int ix_write_character(char *destination, IX_Code_Point code_point);
template<>
int ix_write_character(char16_t *destination, IX_Code_Point code_point);

#endif
