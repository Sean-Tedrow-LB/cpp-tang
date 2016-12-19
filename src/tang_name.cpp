#include "tang_name.hpp"

Tang_Name::~Tang_Name()
{
    if(!data)
    {
        return;
    }
    switch(type)
    {
    case TANG_NAME_CONSTANT:
        delete constant_data();
        break;
    case TANG_NAME_GROUP:
        delete group_data();
        break;
    case TANG_NAME_CLASS:
    case TANG_NAME_FORM:
        delete structure_data();
        break;
    case TANG_NAME_ALIAS:
        delete alias_data();
        break;
    case TANG_NAME_FUNCTION:
        delete function_data();
        break;
    case TANG_NAME_INDETERMINATE:
        delete indeterminate_data();
        break;
    case TANG_NAME_VARIABLE:
        delete variable_data();
    default:
        break;
    }
}
