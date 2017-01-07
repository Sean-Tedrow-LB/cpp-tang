#ifndef __TANG_CHARACTER_MAP__
#define __TANG_CHARACTER_MAP__

#include <map>
#include "tang_part.hpp"
#include <string>

typedef char Tang_Character_Map[256];

#define TANG_CHARACTER_MAP_GETTER(thing) \
    char lookup_##thing(char c) const \
    { \
        return thing##_map[(unsigned char)c]; \
    }

struct Tang_Character_Compendium
{
    Tang_Character_Map   word_map, 
                         number_literal_map, // NOTE: doesn't include '.'
                         char_escape_map,
                         punctuation_map,
                         part_type_map,
                         hex_map;
    
    std::map<std::string, Tang_Part_Punctuation> punctuation_type_map;
    
    Tang_Character_Compendium();
    
    TANG_CHARACTER_MAP_GETTER(word)
    TANG_CHARACTER_MAP_GETTER(number_literal)
    TANG_CHARACTER_MAP_GETTER(char_escape)
    TANG_CHARACTER_MAP_GETTER(punctuation)
    TANG_CHARACTER_MAP_GETTER(part_type)
    
    Tang_Part_Punctuation lookup_punctuation_type(const std::string &str) const;
};

#endif
