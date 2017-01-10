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
        ( \
            ((c > 0x40) && (c < 0x5B)) || \
            ((c > 0x60) && (c < 0x7B)) || \
            (c == '_') \
        ) ? (char)TANG_PART_WORD : \
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
    
    ADD_TO_MAP(punc_map_1, ASSIGN)
    ADD_TO_MAP(punc_map_1, ACCESS)
    ADD_TO_MAP(punc_map_1, COMMA)
    ADD_TO_MAP(punc_map_1, NOT)
    ADD_TO_MAP(punc_map_1, BAND)
    ADD_TO_MAP(punc_map_1, BOR)
    ADD_TO_MAP(punc_map_1, BXOR)
    ADD_TO_MAP(punc_map_1, MOD)
    ADD_TO_MAP(punc_map_1, MULT)
    ADD_TO_MAP(punc_map_1, DIV)
    ADD_TO_MAP(punc_map_1, ADD)
    ADD_TO_MAP(punc_map_1, SUB)
    ADD_TO_MAP(punc_map_1, QM)
    ADD_TO_MAP(punc_map_1, GT)
    ADD_TO_MAP(punc_map_1, LT)
    
    ADD_TO_MAP(punc_map_2, DASSIGN)
    ADD_TO_MAP(punc_map_2, POINT)
    ADD_TO_MAP(punc_map_2, DPOINT)
    ADD_TO_MAP(punc_map_2, ADDRESS)
    ADD_TO_MAP(punc_map_2, GACCESS)
    ADD_TO_MAP(punc_map_2, DO)
    ADD_TO_MAP(punc_map_2, EQ)
    ADD_TO_MAP(punc_map_2, NEQ)
    ADD_TO_MAP(punc_map_2, GTEQ)
    ADD_TO_MAP(punc_map_2, LTEQ)
    ADD_TO_MAP(punc_map_2, LAND)
    ADD_TO_MAP(punc_map_2, LOR)
    ADD_TO_MAP(punc_map_2, SHR)
    ADD_TO_MAP(punc_map_2, SHL)
    ADD_TO_MAP(punc_map_2, BANDASSIGN)
    ADD_TO_MAP(punc_map_2, BORASSIGN)
    ADD_TO_MAP(punc_map_2, BXORASSIGN)
    ADD_TO_MAP(punc_map_2, MODASSIGN)
    ADD_TO_MAP(punc_map_2, MULTASSIGN)
    ADD_TO_MAP(punc_map_2, DIVASSIGN)
    ADD_TO_MAP(punc_map_2, ADDASSIGN)
    ADD_TO_MAP(punc_map_2, SUBASSIGN)
    
    ADD_TO_MAP(punc_map_3, SHRASSIGN)
    ADD_TO_MAP(punc_map_3, SHLASSIGN)
}

Tang_Part_Punctuation 
Tang_Character_Compendium::lookup_punctuation_type(const std::string &str,
                                                   int &length_out) const
{
    std::string s;
    s.resize(3);
    if((int)str.length() >= 1)
    {
        s[0] = str[0];
        if((int)str.length() >= 2)
        {
            s[1] = str[1];
            if((int)str.length() >= 3)
            {
                s[2] = str[2];
                auto p = punc_map_3.find(s);
                if(p != punc_map_3.end())
                {
                    length_out = 3;
                    return p->second;
                }
            }
            s.resize(2);
            auto p = punc_map_2.find(s);
            if(p != punc_map_2.end())
            {
                length_out = 2;
                return p->second;
            }
        }
        s.resize(1);
        auto p = punc_map_1.find(s);
        if(p != punc_map_1.end())
        {
            length_out = 1;
            return p->second;
        }
    }
    return TANG_P_UNRECOGNIZED;
}
