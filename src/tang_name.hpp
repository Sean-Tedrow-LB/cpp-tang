#ifndef __TANG_NAME__
#define __TANG_NAME__

struct Tang_Name;

#include <string>
#include "tang_fundamental_types.hpp"
#include "tang_block_template.hpp"
#include <vector>
#include <new>

// TODO: also need to support builtins?  Actually maybe I don't

enum Tang_Name_Type
{
    TANG_NAME_CONSTANT,
    TANG_NAME_GROUP,
    TANG_NAME_CLASS,
    TANG_NAME_FORM,
    TANG_NAME_ALIAS,
    TANG_NAME_FUNCTION,
    TANG_NAME_INDETERMINATE,
    TANG_NAME_VARIABLE
};

struct Tang_Name_Constant // async
{
    std::string value;
};

struct Tang_Name_Group // async
{
    std::map<std::string, Tang_Name>   names;
    Tang_Block_Template               *imported_block = nullptr;
};

struct Tang_Name_Structure_Synonym
{
    std::string base_string;
    Tang_Name_Structure_Synonym  *base_structure = nullptr;
    Tang_Block_Template          *block          = nullptr;
    int                           origin_id      = -1;
};

struct Tang_Name_Structure // async
{
    std::vector<Tang_Name_Structure_Synonym> synonyms;
};

struct Tang_Name_Alias_Indeterminate
{
    std::string name, 
                default_value;
    int         out_position;
};

// TODO: ALIASES NEED SYNONYMS?
struct Tang_Name_Alias //async
{
    std::vector<Tang_Name_Alias_Indeterminate>   indeterminate_list;
    Tang_Name                                   *name   = nullptr;
    Tang_Fundamental_Type                        ftype  = TANG_FTYPE_UNSET;
    bool                                         is_c   = false;
};

struct Tang_Name_Function_Synonym
{
    Tang_Block_Template  *block     = nullptr;
    int                   origin_id = -1;
    bool                  is_c      = false;
};

struct Tang_Name_Function // async
{
    std::vector<Tang_Name_Function_Synonym> synonyms;
};

struct Tang_Name_Indeterminate // sync
{
    std::string default_determinate;
    // TODO
};

struct Tang_Name_Variable // sync
{
    std::string type_string, 
                default_value;
    // NOTE: type_string is used temporarily for function parameters, until
    //       the sync pass gets here.
    // TODO
};


#define TANG_NAME_GETTER(thing_name, Thing, pointer) \
    Tang_Name_##Thing* thing_name##_data() \
    { \
        return (Tang_Name_##Thing*)pointer; \
    } \
    const Tang_Name_##Thing* thing_name##_data() const \
    { \
        return (const Tang_Name_##Thing*)pointer; \
    }

#define TANG_NAME_INITIALIZER(thing_name, THING_ENUM, \
                              the_enum, Thing, pointer) \
    void init_##thing_name() \
    { \
        the_enum = TANG_NAME_##THING_ENUM; \
        pointer = (void*)(new Tang_Name_##Thing); \
    }

struct Tang_Name
{
    std::string name;
    int line_number;
    Tang_Name_Type type;
    void *data;
    
    TANG_NAME_GETTER(constant, Constant, data)
    TANG_NAME_GETTER(group, Group, data)
    TANG_NAME_GETTER(structure, Structure, data)
    TANG_NAME_GETTER(alias, Alias, data)
    TANG_NAME_GETTER(function, Function, data)
    TANG_NAME_GETTER(indeterminate, Indeterminate, data)
    TANG_NAME_GETTER(variable, Variable, data)
    
    TANG_NAME_INITIALIZER(constant, CONSTANT, type, Constant, data)
    TANG_NAME_INITIALIZER(group, GROUP, type, Group, data)
    TANG_NAME_INITIALIZER(form, FORM, type, Structure, data)
    TANG_NAME_INITIALIZER(class, CLASS, type, Structure, data)
    TANG_NAME_INITIALIZER(alias, ALIAS, type, Alias, data)
    TANG_NAME_INITIALIZER(function, FUNCTION, type, Function, data)
    TANG_NAME_INITIALIZER(indeterminate, INDETERMINATE, type, Indeterminate, data)
    TANG_NAME_INITIALIZER(variable, VARIABLE, type, Variable, data)
    
    Tang_Name(Tang_Name &&src)
    {
        data = src.data;
        src.data = nullptr;
    }
    Tang_Name()
    {
        // PASS
    }
    ~Tang_Name();
};


#endif
