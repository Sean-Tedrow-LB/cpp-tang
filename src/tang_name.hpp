#ifndef __TANG_NAME__
#define __TANG_NAME__

#include <string>



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

struct Tang_Name_Indeterminate
{
    // TODO
};

struct Tang_Name_Variable
{
    // TODO
};

struct Tang_Name_Constant
{
    // TODO
};

struct Tang_Name_Group
{
    // TODO
};

struct Tang_Name_Structure
{
    // TODO
};

struct Tang_Name_Alias
{
    // TODO
};

struct Tang_Name_Function
{
    // TODO
};

union Tang_Name_Union
{
    Tang_Name_Indeterminate   indeterminate;
    Tang_Name_Variable        variable;
    Tang_Name_Constant        constant;
    Tang_Name_Group           group;
    Tang_Name_Structure       structure;
    Tang_Name_Alias           alias;
    Tang_Name_Function        function;
};

struct Tang_Name_Base
{
    std::string name;
    int line_number = 0;
    Tang_Name_Type type;
    Tang_Name_Union data_union;
    
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

#endif
