#include "tang_part.hpp"
#include "tang_character_map.hpp"

#define TANG_SET_UP_CHARACTER_MAP(map, FUNCTION) \
    for(int i = 0; i < 256; i++) \
    { \
        map[i] = FUNCTION(i); \
    }

#define TANG_WORD_FUNCTION(c) ( \
                                ( \
                                  ((c >= 0x30) && (c < 0x3A)) || \
                                  ((c > 0x40) && (c < 0x5B)) || \
                                  ((c > 0x60) && (c < 0x7B)) \
                                ) ? c : \
                                ( \
                                  ((c == '_') || (c == '-')) ? '_' : '\0' \
                                ) \
                              )

#define TANG_NUMBER_LITERAL_FUNCTION(c) ( \
                                          ((c >= 0x30) && (c < 0x3A)) || \
                                          ((c > 0x40) && (c < 0x47)) || \
                                          ((c > 0x60) && (c < 0x67)) \
                                        )

#define TANG_CHAR_ESCAPE_FUNCTION(c) ( \
                                       (c == 'n') ? '\n' : \
                                       ( \
                                         (c == 'r') ? '\r' : \
                                         ( \
                                           (c == 't') ? '\t' : c \
                                         ) \
                                       ) \
                                     )

#define TANG_PUNCTUATION_FUNCTION(c) ( \
                                       (c == '!') || (c == '%') || \
                                       (c == '&') || \
                                       ((c >= 0x2A) && (c < 0x30)) || \
                                       ((c >= 0x3A) && (c < 0x40)) || \
                                       (c == '^') || (c == '|') \
                                     )

#define TANG_PART_TYPE_FUNCTION(c) \
    ( \
      ((c >= 0x30) && (c < 0x3A)) ? (char)TANG_PART_NUMBER : \
      ( \
        TANG_WORD_FUNCTION(c) ? (char)TANG_PART_WORD : \
        ( \
          (c == '\'') ? (char)TANG_PART_CHARACTER : \
          ( \
            (c == '\"') ? (char)TANG_PART_STRING : \
            ( \
              ( \
                (c == '(') || (c == ')') || \
                (c == '[') || (c == ']') || \
                (c == '{') || (c == '}') \
              ) ? (char)TANG_PART_ENCLOSER : \
              ( \
                TANG_PUNCTUATION_FUNCTION(c) ? \
                  (char)TANG_PART_PUNCTUATION : (char)TANG_PART_ERRONEOUS \
              ) \
            ) \
          ) \
        ) \
      ) \
    )

#define ADD_TO_MAP(map, THING)   map[TANG_PS_##THING] = TANG_P_##THING;

Tang_Character_Compendium::Tang_Character_Compendium()
{
    TANG_SET_UP_CHARACTER_MAP(word_map, TANG_WORD_FUNCTION)
    TANG_SET_UP_CHARACTER_MAP(number_literal_map, TANG_NUMBER_LITERAL_FUNCTION)
    TANG_SET_UP_CHARACTER_MAP(char_escape_map, TANG_CHAR_ESCAPE_FUNCTION)
    TANG_SET_UP_CHARACTER_MAP(punctuation_map, TANG_PUNCTUATION_FUNCTION)
    TANG_SET_UP_CHARACTER_MAP(part_type_map, TANG_PART_TYPE_FUNCTION)
    
    
    ADD_TO_MAP(punctuation_type_map, ASSIGN)
    ADD_TO_MAP(punctuation_type_map, DASSIGN)
    ADD_TO_MAP(punctuation_type_map, POINT)
    ADD_TO_MAP(punctuation_type_map, DPOINT)
    ADD_TO_MAP(punctuation_type_map, ADDRESS)
    ADD_TO_MAP(punctuation_type_map, ACCESS)
    ADD_TO_MAP(punctuation_type_map, GACCESS)
    ADD_TO_MAP(punctuation_type_map, DO)
    ADD_TO_MAP(punctuation_type_map, COMMA)
    ADD_TO_MAP(punctuation_type_map, EQ)
    ADD_TO_MAP(punctuation_type_map, NEQ)
    ADD_TO_MAP(punctuation_type_map, GT)
    ADD_TO_MAP(punctuation_type_map, LT)
    ADD_TO_MAP(punctuation_type_map, GTEQ)
    ADD_TO_MAP(punctuation_type_map, LTEQ)
    ADD_TO_MAP(punctuation_type_map, NOT)
    ADD_TO_MAP(punctuation_type_map, LAND)
    ADD_TO_MAP(punctuation_type_map, LOR)
    ADD_TO_MAP(punctuation_type_map, BAND)
    ADD_TO_MAP(punctuation_type_map, BOR)
    ADD_TO_MAP(punctuation_type_map, BXOR)
    ADD_TO_MAP(punctuation_type_map, SHR)
    ADD_TO_MAP(punctuation_type_map, SHL)
    ADD_TO_MAP(punctuation_type_map, MOD)
    ADD_TO_MAP(punctuation_type_map, MULT)
    ADD_TO_MAP(punctuation_type_map, DIV)
    ADD_TO_MAP(punctuation_type_map, ADD)
    ADD_TO_MAP(punctuation_type_map, SUB)
    ADD_TO_MAP(punctuation_type_map, BANDASSIGN)
    ADD_TO_MAP(punctuation_type_map, BORASSIGN)
    ADD_TO_MAP(punctuation_type_map, BXORASSIGN)
    ADD_TO_MAP(punctuation_type_map, SHRASSIGN)
    ADD_TO_MAP(punctuation_type_map, SHLASSIGN)
    ADD_TO_MAP(punctuation_type_map, MODASSIGN)
    ADD_TO_MAP(punctuation_type_map, MULTASSIGN)
    ADD_TO_MAP(punctuation_type_map, DIVASSIGN)
    ADD_TO_MAP(punctuation_type_map, ADDASSIGN)
    ADD_TO_MAP(punctuation_type_map, SUBASSIGN)
    ADD_TO_MAP(punctuation_type_map, QM)
    ADD_TO_MAP(punctuation_type_map, COLON)
}

Tang_Part_Punctuation 
Tang_Character_Compendium::lookup_punctuation_type(const std::string &str) const
{
    auto p = punctuation_type_map.find(str);
    if(p == punctuation_type_map.end())
    {
        return TANG_P_UNRECOGNIZED;
    }
    else
    {
        return p->second;
    }
}
