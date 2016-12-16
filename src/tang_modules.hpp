#ifndef __TANG_MODULES__
#define __TANG_MODULES__

#include "tang_block.hpp"
#include <map>
#include <vector>
#include <string>
#include "tang_comment_removal.hpp"
#include <new>

struct Tang_Module_Node
{
    std::string name;
    std::map<std::string, Tang_Module_Node>  children;
    Tang_Text_Without_Comments               text;
    Tang_Module_Node                        *parent       = nullptr;
    Tang_Block                              *root_block   = nullptr;
    
    ~Tang_Module_Node()
    {
        delete root_block;
    }
    
    std::string get_dir_path() const;
    std::string get_module_path() const;
};

struct Tang_Module_Tracker
{
    Tang_Module_Node root;
    // on windows, the library directory might be on a separate drive.
    #ifdef _WIN32
    Tang_Module_Node library_root;
    bool library_root_needed;
    #endif
    std::vector<Tang_Module_Node*> modules;
    Tang_Module_Node *working_directory_node,
                     *library_directory_node;
    
    bool initialize();
    
    Tang_Module_Node* get_module_from_module(const char *path, char delimiter,
                                             Tang_Module_Node *current_node,
                                             bool &is_new_out);
    
    Tang_Module_Node* load_module_from_working_dir(const std::string &path);
    Tang_Module_Node* load_module_from_library_dir(const std::string &path);
    Tang_Module_Node* load_module_from_module(const std::string &path, 
                                              Tang_Module_Node *source_node);
};

#endif
