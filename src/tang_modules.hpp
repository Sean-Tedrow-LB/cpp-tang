#ifndef __TANG_MODULES__
#define __TANG_MODULES__

#include "tang_block.hpp"
#include <map>
#include <vector>
#include <string>

struct Tang_Module_Node
{
    std::string name;
    std::map<std::string, Tang_Module_Node> children;
    Tang_Module_Node *parent = nullptr;
    Tang_Block *root_block = nullptr;
    
    std::string get_path();
};

struct Tang_Module_Tracker
{
    Tang_Module_Node root;
    // on windows, the library directory might be on a separate drive.
    #ifdef _WIN32
    Tang_Module_Node library_root;
    bool library_root_needed;
    #endif
    Tang_Module_Node *working_directory_node,
                     *library_directory_node;
    
    bool initialize();
    
    Tang_Module_Node* load_module_from_working_dir(const std::string &path);
    Tang_Module_Node* load_module_from_library_dir(const std::string &path);
    Tang_Module_Node* load_module_from_module(const std::string &path, 
                                              Tang_Module_Node *source_node);
};

#endif
