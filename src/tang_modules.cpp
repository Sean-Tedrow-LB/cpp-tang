#include "tang_modules.hpp"
#include <iostream>




#ifdef _WIN32

#include <Windows.h>
#include <cstring>
#include "ix_unicode.hpp"
#include <climits>

static bool env_to_string(const char *env_name, std::string &string_out)
{
    std::u16string wout, wenv_name;
    int env_name_length = (int)strlen(env_name);
    int wenv_name_length = ix_measure_utf8_to_utf16(env_name, env_name_length);
    wenv_name.resize(wenv_name_length);
    ix_convert_utf8_to_utf16(&(wenv_name[0]), env_name, env_name_length);
    DWORD wout_length = INT_MAX;
    for(DWORD sz = 256; sz < wout_length; sz *= 2)
    {
        wout.resize(sz - 1);
        wout_length = GetEnvironmentVariable((const wchar_t*)wenv_name.c_str(),
                                             (wchar_t*)&(wout[0]), sz);
        if(wout_length = 0)
        {
            return false;
        }
    }
    int out_length = ix_measure_utf16_to_utf8(wout.c_str(), wout_length);
    string_out.resize(out_length);
    ix_convert_utf16_to_utf8(&(string_out[0]), wout.c_str(), wout_length);
    return true;
}

static std::string get_working_directory()
{
    // TODO
}




#define PATH_DELIMITER '\\'
#define DEFAULT_TANG_LIBRARY_PATH   "/usr/tang-libraries"

#else

#include <unistd.h>
#include <cstring>
#include <cstdlib>

static bool env_to_string(const char *env_name, std::string &string_out)
{
    const char *env = getenv(env_name);
    if(!env)
    {
        return false;
    }
    string_out = env;
    return true;
}

static std::string get_working_directory()
{
    std::string return_value;
    for(int sz = 256;;sz *= 2)
    {
        return_value.resize(sz);
        const char *status = getcwd(&(return_value[0]), sz);
        if(status)
        {
            return_value.resize(strlen(return_value.data()));
            return return_value;
        }
    }
}

#define PATH_DELIMITER   '/'
#define DEFAULT_TANG_LIBRARY_PATH   "/usr/tang-libraries"

#endif

std::string Tang_Module_Node::do_get_path() const
{
    if(parent)
    {
        return parent->do_get_path() + PATH_DELIMITER + name;
    }
    else
    {
        return name;
    }
}

std::string Tang_Module_Node::get_path() const
{
    if(parent)
    {
        return parent->do_get_path() + PATH_DELIMITER + name + ".tang";
    }
    else
    {
        return name + ".tang";
    }
}


// NOTE: returns true when you've reached the end of the path
template <char DELIMITER>
bool get_step(const char *path, std::string &out)
{
    for(int i = 0;; i++)
    {
        char c = path[i];
        bool reached_end = !c;
        if(c == DELIMITER || reached_end)
        {
            out.resize(i);
            for(int j = 0; j < i; j++)
            {
                out[j] = path[j];
            }
            return reached_end;
        }
    }
}

template <char DELIMITER>
Tang_Module_Node* 
Tang_Module_Tracker::get_module_from_module(const char *p,
                                            Tang_Module_Node *current_node,
                                            bool &is_new_out)
{
    std::string step;
    is_new_out = false;
    bool is_new = false;
    bool end_reached = false;
    do
    {
        end_reached = get_step<DELIMITER>(p, step);
        p += step.length() + 1;
        if(step == "..")
        {
            current_node = current_node->parent;
            if(!current_node)
            {
                return nullptr;
            }
            is_new = false;
        }
        else
        {
            Tang_Module_Node *next_node = &(current_node->children[step]);
            if(!next_node->parent)
            {
                is_new = true;
                next_node->parent = current_node;
                next_node->name = step;
            }
            else
            {
                is_new = false;
            }
            current_node = next_node;
        }
    }
    while(!end_reached);
    is_new_out = is_new;
    return current_node;
}


bool Tang_Module_Tracker::initialize()
{
    // TODO: CLEAN THIS FUNCTION THE FUCK UP.
    std::string lib_path;
    bool lib_path_found = env_to_string("TANG_LIBRARY_PATH", lib_path);
    if(!lib_path_found)
    {
        std::cout << "No TANG_LIBRARY_PATH environment variable found, "
                     "defaulting to \"" << DEFAULT_TANG_LIBRARY_PATH <<
                     "\"" << std::endl;
        
        lib_path = DEFAULT_TANG_LIBRARY_PATH;
    }
    std::string wd_path = get_working_directory();
    if(wd_path.empty())
    {
        std::cout << "failed to get path" << std::endl;
        return false;
    }
    bool is_new;
#ifdef _WIN32
    std::string lib_step, wd_step;
    const char *lib_path_p = &(lib_path[0]);
    const char *wd_path_p = &(wd_path[0]);
    bool lib_path_just_root = get_step<PATH_DELIMITER>(lib_path_p, lib_step);
    bool wd_path_just_root = get_step<PATH_DELIMITER>(wd_path_p, wd_step);
    lib_path_p += lib_step.length() + 1;
    wd_path_p += wd_step.length() + 1;
    root.name = wd_step;
    if(lib_step != wd_step)
    {
        library_root_needed = true;
        library_root.name = lib_step;
        if(!lib_path_just_root)
        {
            library_directory_node = get_module_from_module<PATH_DELIMITER>(lib_path_p, 
                                                                            &library_root, 
                                                                            is_new);
        }
        else
        {
            library_directory_node = &library_root;
        }
    }
    else
    {
        if(!lib_path_just_root)
        {
            library_directory_node = get_module_from_module<PATH_DELIMITER>(lib_path_p, 
                                                                            &root, 
                                                                            is_new);
        }
        else
        {
            library_directory_node = &root;
        }
    }
#else
    bool lib_path_from_root = lib_path[0] == '/';
    bool wd_path_from_root = wd_path[0] == '/';
    if(!lib_path_from_root || !wd_path_from_root)
    {
        if(!lib_path_from_root)
        {
            std::cout << "Given library path, \"" << lib_path
                      << "\" is not from root (it should be)" << std::endl;
        }
        if(!wd_path_from_root)
        {
            std::cout << "Given working directory path, \"" << lib_path
                      << "\" is not from root (it should be)" << std::endl;
        }
        return false;
    }
    bool lib_path_just_root = lib_path.length() == 1;
    bool wd_path_just_root = wd_path.length() == 1;
    
    const char *lib_path_p = &(lib_path[1]);
    const char *wd_path_p = &(wd_path[1]);
    if(!lib_path_just_root)
    {
        library_directory_node = get_module_from_module<PATH_DELIMITER>(lib_path_p, 
                                                                        &root, 
                                                                        is_new);
    }
    else
    {
        library_directory_node = &root;
    }
#endif
    if(!wd_path_just_root)
    {
        working_directory_node = get_module_from_module<PATH_DELIMITER>(wd_path_p, 
                                                                        &root, 
                                                                        is_new);
    }
    else
    {
        working_directory_node = &root;
    }
    (void)is_new;
    return true;
}




Tang_Module_Node* 
Tang_Module_Tracker::load_module_from_working_dir(const std::string &path)
{
    return load_module_from_module(path, working_directory_node);
}

Tang_Module_Node* 
Tang_Module_Tracker::load_module_from_library_dir(const std::string &path)
{
    return load_module_from_module(path, library_directory_node);
}


Tang_Module_Node* 
Tang_Module_Tracker::load_module_from_module(const std::string &path, 
                                             Tang_Module_Node *current_node)
{
    if(path.empty())
    {
        std::cout << "Path to module is empty" << std::endl;
        return nullptr;
    }
    bool is_new = false;
    current_node = get_module_from_module<'/'>(path.c_str(), 
                                               current_node, 
                                               is_new);
    if(is_new)
    {
        Tang_File_Reader reader;
        std::string path = current_node->get_path();
        if(!reader.open(path))
        {
            return nullptr;
        }
        std::cout << "Loading module \"" << path << "\"" << std::endl;
        current_node->text.from(reader);
        current_node->root_block = new Tang_Block;
        modules.push_back(current_node);
    }
    return current_node;
}
