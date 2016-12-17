#ifndef __TANG_NAME__
#define __TANG_NAME__

struct Tang_Name;

#include <string>
#include "tang_funamental_types.hpp"
#include "tang_block_template.hpp"
#include <vector>

// TODO: need to support synonyms, addons
// TODO: also need to support builtins

enum Tang_Name_Type
{
    TANG_NAME_INDETERMINATE,
    TANG_NAME_VARIABLE,
    TANG_NAME_CONSTANT,
    TANG_NAME_GROUP,
    TANG_NAME_CLASS,
    TANG_NAME_FORM,
    TANG_NAME_ALIAS,
    TANG_NAME_FUNCTION
};

struct Tang_Name
{
    std::string name;
    int line_number = 0;
    Tang_Name_Type type;
    void *data;
    
    void init_indeterminate(/* TODO */);
    void init_variable(/* TODO */);
    void init_constant(/* TODO */);
    void init_group(/* TODO */);
    void init_structure(/* TODO */);
    void init_alias(/* TODO */);
    void init_function(/* TODO */);
    
    ~Tang_Name_Base()
    {
        // TEMPORARY
    }
};






struct Tang_Name_Constant // async
{
    std::string value;
};

struct Tang_Name_Group // async
{
    std::map<std::string, Tang_Name> names;
    Tang_Block_Template *imported_block;
};

struct Tang_Name_Structure_Synonym
{
    Tang_Block_Template *block;
    int origin_id;
};

struct Tang_Name_Structure // async
{
    std::vector<Tang_Name_Structure_Synonym> synonyms;
};

struct Tang_Name_Alias_Indeterminate
{
    std::string name, default_value;
    int out_position;
};

struct Tang_Name_Alias //async
{
    Tang_Fundamental_Type                        ftype;
    Tang_Name                                   *name;
    std::string                                  c_type_name;
    std::vector<Tang_Name_Alias_Indeterminate>   indeterminate_list;
};

struct Tang_Name_Function_Synonym
{
    Tang_Block_Template *block;
    int origin_id;
    bool is_c;
}

struct Tang_Name_Function // async
{
    std::vector<Tang_Name_Function_Synonym> synonyms;
};

struct Tang_Name_Indeterminate // sync
{
    // TODO
};

struct Tang_Name_Variable // sync
{
    // TODO
};

#endif
