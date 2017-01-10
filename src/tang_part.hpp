#ifndef __TANG_PART__
#define __TANG_PART__

#include "ix_unicode.hpp"
#include <string>
#include <map>

enum Tang_Part_Type
{
    TANG_PART_WORD,
    TANG_PART_NUMBER,
    TANG_PART_STRING,
    TANG_PART_CHARACTER,
    TANG_PART_PUNCTUATION,
    TANG_PART_ENCLOSER,
    TANG_PART_ERRONEOUS
};

#define TANG_PS_ASSIGN      "="
#define TANG_PS_DASSIGN     ":="
#define TANG_PS_POINT       "->"
#define TANG_PS_DPOINT      ":>"
#define TANG_PS_ADDRESS     "<-"
#define TANG_PS_ACCESS      "."
#define TANG_PS_GACCESS     "::"
#define TANG_PS_DO          ";;"
#define TANG_PS_COMMA       ","
#define TANG_PS_EQ          "=="
#define TANG_PS_NEQ         "!="
#define TANG_PS_GT          ">"
#define TANG_PS_LT          "<"
#define TANG_PS_GTEQ        ">="
#define TANG_PS_LTEQ        "<="
#define TANG_PS_NOT         "!"
#define TANG_PS_LAND        "&&"
#define TANG_PS_LOR         "||"
#define TANG_PS_BAND        "&"
#define TANG_PS_BOR         "|"
#define TANG_PS_BXOR        "^"
#define TANG_PS_SHR         ">>"
#define TANG_PS_SHL         "<<"
#define TANG_PS_MOD         "%"
#define TANG_PS_MULT        "*"
#define TANG_PS_DIV         "/"
#define TANG_PS_ADD         "+"
#define TANG_PS_SUB         "-"
#define TANG_PS_BANDASSIGN  "&="
#define TANG_PS_BORASSIGN   "|="
#define TANG_PS_BXORASSIGN  "^="
#define TANG_PS_SHRASSIGN   ">>="
#define TANG_PS_SHLASSIGN   "<<="
#define TANG_PS_MODASSIGN   "%="
#define TANG_PS_MULTASSIGN  "*="
#define TANG_PS_DIVASSIGN   "/="
#define TANG_PS_ADDASSIGN   "+="
#define TANG_PS_SUBASSIGN   "-="
#define TANG_PS_QM          "?"


enum Tang_Part_Punctuation
{
    TANG_P_ASSIGN,
    TANG_P_DASSIGN,
    TANG_P_POINT,
    TANG_P_DPOINT,
    TANG_P_ADDRESS,
    TANG_P_ACCESS,
    TANG_P_GACCESS,
    TANG_P_DO,
    TANG_P_COMMA,
    TANG_P_EQ,
    TANG_P_NEQ,
    TANG_P_GT,
    TANG_P_LT,
    TANG_P_GTEQ,
    TANG_P_LTEQ,
    TANG_P_NOT,
    TANG_P_LAND,
    TANG_P_LOR,
    TANG_P_BAND,
    TANG_P_BOR,
    TANG_P_BXOR,
    TANG_P_SHR,
    TANG_P_SHL,
    TANG_P_MOD, 
    TANG_P_MULT,
    TANG_P_DIV,
    TANG_P_ADD,
    TANG_P_SUB,
    TANG_P_BANDASSIGN,
    TANG_P_BORASSIGN,
    TANG_P_BXORASSIGN,
    TANG_P_SHRASSIGN,
    TANG_P_SHLASSIGN,
    TANG_P_MODASSIGN,
    TANG_P_MULTASSIGN,
    TANG_P_DIVASSIGN,
    TANG_P_ADDASSIGN,
    TANG_P_SUBASSIGN,
    TANG_P_QM,
    TANG_P_UNRECOGNIZED
};

struct Tang_Part
{
    Tang_Part_Type type;
    std::string value_string;
    Tang_Part_Punctuation punctuation;
    IX_Code_Point code_point;
    int line_number;
};

#endif
