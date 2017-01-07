#ifndef __TANG_NUMBER__
#define __TANG_NUMBER__

#include <vector>


struct Tang_Unit_Hex
{
    char u[2];
    char& operator[](int i)
    {
        return u[i];
    }
    char operator[](int i) const
    {
        return u[i];
    }
};

typedef std::vector<Tang_Unit_Hex> Tang_Unit_Hex_String;

// NOTE: the hex being read must be integer, must be positive, and
//       must already be confirmed to be valid hex
int tang_read_hex(const char *src, int src_len);


void tang_unit_as_hex(char unit, Tang_Unit_Hex &hex_out);
Tang_Unit_Hex_String tang_units_as_hex(const char *units, int unit_count);


// TODO: more number functions will be needed at some point


#endif
