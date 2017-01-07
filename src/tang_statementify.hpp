#ifndef __TANG_STATEMENTIFY__
#define __TANG_STATEMENTIFY__

#include "tang_modules.hpp"
#include <vector>

// NOTE: tabs_out will hold the line number of each tab found in the module.
void statementify_module(Tang_Module_Node *module, 
                         std::vector<int> &tabs_out);

#endif
