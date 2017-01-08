#ifndef __TANG_OUTPUT__
#define __TANG_OUTPUT__

#include "tang_modules.hpp"
#include "tang_file_writer.hpp"

void output_text_without_comments(const Tang_Module_Tracker &module_tracker,
                                  Tang_File_Writer &writer);
void output_blocks_and_statements(const Tang_Module_Tracker &module_tracker,
                                  Tang_File_Writer &writer);
void output_parts(const Tang_Module_Tracker &module_tracker,
                  Tang_File_Writer &writer);

#endif
