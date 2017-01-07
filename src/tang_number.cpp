#include "tang_number.hpp"

int tang_read_hex(const char *src, int src_len)
{
    int j = 0;
    for(int i = 0; i < src_len; i++)
    {
        j *= 0x10;
        char c = src[i];
        if((c >= 0x30) && (c < 0x3A))
        {
            j += (c - 0x30);
        }
        else if((c > 0x40) && (c <= 0x46))
        {
            j += (c - 0x41 + 10);
        }
        else if((c > 0x60) && (c <= 0x66))
        {
            j += (c - 0x61 + 10);
        }
    }
    return j;
}

#define DIGIT_TO_HEX(n)   ((n < 0x10) ? (n + 0x30) : (n + 0x41))

void tang_unit_as_hex(char unit, Tang_Unit_Hex &hex_out)
{
    unsigned char c = (unsigned char)(unit);
    char b = (char)(c % 0x10);
    char a = (char)(c / 0x10);
    hex_out[0] = DIGIT_TO_HEX(a);
    hex_out[1] = DIGIT_TO_HEX(b);
}

Tang_Unit_Hex_String tang_units_as_hex(const char *units, int unit_count)
{
    Tang_Unit_Hex_String out;
    out.resize(unit_count);
    for(int i = 0; i < unit_count; i++)
    {
        tang_unit_as_hex(units[i], out[i]);
    }
    return out;
}
