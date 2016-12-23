#include "tang_compilation.hpp"
#include "tang_async_pass.hpp"

bool tang_compile(Tang_Module_Tracker &module_tracker)
{
    tang_async_pass(module_tracker);
    return true;
}
