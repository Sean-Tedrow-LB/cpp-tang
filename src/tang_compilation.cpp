#include "tang_compilation_passes.hpp"
#include "tang_name.hpp"

class Tang_Compiler_Data
{
    std::vector<Tang_Name*> c_functions;
    Tang_Name *run_function;
    bool too_many_run_functions;
    
    void module_async_pass(Tang_Module_Tracker &tracker, 
                           Tang_Module_Node *module);
    void async_pass(Tang_Module_Tracker &tracker);
    void sync_pass(Tang_Module_Tracker &tracker);
    bool compile_pass(const std::string &out_path);
};


void Tang_Compiler_Data::module_async_pass(Tang_Module_Tracker &tracker, 
                                           Tang_Module_Node *module)
{
    // TODO
}
void Tang_Compiler_Data::async_pass(Tang_Module_Tracker &tracker)
{
    for(int i = 0; i < tracker.modules.size(); i++)
    {
        module_async_pass(tracker, tracker.modules[i]);
    }
}
void Tang_Compiler_Data::sync_pass(Tang_Module_Tracker &tracker)
{
    // TODO
}
bool Tang_Compiler_Data::compile_pass(const std::string &out_path)
{
    // TODO
    return false;
}


bool tang_compile(Tang_Module_Tracker &module_tracker, 
                  const std::string &out_path)
{
    Tang_Compiler_Data compiler_data;
    compiler_data.async_pass(module_tracker);
    compiler_data.sync_pass(module_tracker);
    return compile_pass(out_path);
}
