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

struct Tang_Name_Base
{
    Tang_Name_Type type;
    std::string name;
    int line_number;
};

void delete_name(Tang_Name_Base *name);

#endif
