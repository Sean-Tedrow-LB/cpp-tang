#include "ix_unicode.hpp"



template<typename UF, typename UT>
void ix_convert_string(UT *destination, const UF *source, int source_length)
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


template<typename UF, typename UT>
int ix_measure_string(const UF *source, int source_length)
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
