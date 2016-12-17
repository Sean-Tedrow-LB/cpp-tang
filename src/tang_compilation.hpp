#ifndef __TANG_COMPILATION__
#define __TANG_COMPILATION__

#include "tang_modules.hpp"
#include <string>

bool tang_compile(Tang_Module_Tracker &module_tracker, 
                  const std::string &out_path);

#endif
