#include "tang_modules.hpp"
#include <iostream>
#include <cstring>



#ifdef _WIN32

#include <Windows.h>
#include "ix_unicode.hpp"
#include "windows_error.hpp"

static bool env_to_string(const char *env_name, std::string &string_out)
{
    std::u16string wout, wenv_name;
    int env_name_length = (int)strlen(env_name);
    int wenv_name_length = ix_measure_utf8_to_utf16(env_name, env_name_length);
    wenv_name.resize(wenv_name_length);
    ix_convert_utf8_to_utf16(&(wenv_name[0]), env_name, env_name_length);
    wout.resize(255);
    DWORD wout_length = GetEnvironmentVariable((const wchar_t*)wenv_name.c_str(),
                                               (wchar_t*)&(wout[0]), 256);
    if(wout_length == 0)
    {
        return false;
    }
    if(wout_length > 256)
    {
        wout.resize(wout_length - 1);
        wout_length = GetEnvironmentVariable((const wchar_t*)wenv_name.c_str(),
                                             (wchar_t*)&(wout[0]), wout_length);
        if(wout_length == 0)
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
    std::u16string wout;
    DWORD wout_length = GetCurrentDirectory(0, nullptr);
    if(wout_length == 0)
    {
        std::cout << "failed to get working directory with error \"" << 
                 windows_error_string() << "\"" << std::endl;
        return std::string();
    }
    wout.resize(wout_length - 1);
    wout_length = GetCurrentDirectory(wout_length, (wchar_t*)&(wout[0]));
    if(wout_length == 0)
    {
        std::cout << "failed to get working directory with error \"" << 
                 windows_error_string() << "\"" << std::endl;
        return std::string();
    }
    int out_length = ix_measure_utf16_to_utf8(wout.c_str(), wout_length);
    std::string string_out;
    string_out.resize(out_length);
    ix_convert_utf16_to_utf8(&(string_out[0]), wout.c_str(), wout_length);
    return string_out;
}


#define PATH_DELIMITER '\\'
#define DEFAULT_TANG_LIBRARY_PATH   "C:\\Program Files\\Tang\\tang-libraries"

#else

#include <unistd.h>
#include <cstdlib>
#include <cerrno>

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
        else
        {
            return std::string();
        }
    }
}

#define PATH_DELIMITER   '/'
#define DEFAULT_TANG_LIBRARY_PATH   "/usr/tang-libraries"

#endif

std::string Tang_Module_Node::get_dir_path() const
{
    if(parent)
    {
        return parent->get_dir_path() + PATH_DELIMITER + name;
    }
    else
    {
        return name;
    }
}

std::string Tang_Module_Node::get_module_path() const
{
    return get_dir_path() + ".tang";
}


// NOTE: returns true when you've reached the end of the path
bool get_step(const char *path, char delimiter, std::string &out)
{
    for(int i = 0;; i++)
    {
        char c = path[i];
        bool reached_end = !c;
        if(c == delimiter || reached_end)
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


Tang_Module_Node* 
Tang_Module_Tracker::get_node_from_node(const char *p, char delimiter,
                                        Tang_Module_Node *current_node,
                                        bool &is_new_out)
{
    std::string step;
    is_new_out = false;
    bool is_new = false;
    bool end_reached = false;
    do
    {
        end_reached = get_step(p, delimiter, step);
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

Tang_Module_Node* 
Tang_Module_Tracker::load_module_from_node(const std::string &path, 
                                             Tang_Module_Node *current_node)
{
    if(path.empty())
    {
        std::cout << "Path to module is empty" << std::endl;
        return nullptr;
    }
    bool is_new = false;
    current_node = get_node_from_node(path.c_str(), '/',
                                      current_node, is_new);
    if(is_new)
    {
        Tang_File_Reader reader;
        std::string path = current_node->get_module_path();
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


bool Tang_Module_Tracker::initialize()
{
    std::string lib_path;
    bool lib_path_found = env_to_string("TANG_LIBRARY_PATH", lib_path);
    if(!lib_path_found)
    {
        std::cout << "No TANG_LIBRARY_PATH environment variable found, "
                     "defaulting to \"" DEFAULT_TANG_LIBRARY_PATH  "\"" 
                     << std::endl;
        lib_path = DEFAULT_TANG_LIBRARY_PATH;
    }
    std::string wd_path = get_working_directory();
    if(wd_path.empty())
    {
        std::cout << "Failed to get working directory" << std::endl;
        return false;
    }
    bool is_new_throwaway, wd_path_just_root;
    
    const char *wd_path_p;
    
    
#ifdef _WIN32
    {
        std::string lib_step, wd_step;
        const char *lib_path_p = &(lib_path[0]);
        wd_path_p = &(wd_path[0]);
        bool lib_path_just_root = get_step(lib_path_p, PATH_DELIMITER, lib_step);
        wd_path_just_root = get_step(wd_path_p, PATH_DELIMITER, wd_step);
        lib_path_p += lib_step.length() + 1;
        wd_path_p += wd_step.length() + 1;
        root.name = wd_step;
        if(lib_step != wd_step)
        {
            library_root.name = lib_step;
            if(lib_path_just_root)
            {
                library_directory_node = &library_root;
            }
            else
            {
                library_directory_node = get_node_from_node(lib_path_p, 
                                                            PATH_DELIMITER, 
                                                            &library_root, 
                                                            is_new_throwaway);
            }
        }
        else
        {
            if(lib_path_just_root)
            {
                library_directory_node = &root;
            }
            else
            {
                library_directory_node = get_node_from_node(lib_path_p, 
                                                            PATH_DELIMITER,
                                                            &root, 
                                                            is_new_throwaway);
            }
        }
    }
    
    
    
#else


    {
        bool lib_path_from_root = lib_path[0] == '/';
        bool wd_path_from_root = wd_path[0] == '/';
        if(!lib_path_from_root || !wd_path_from_root)
        {
            if(!lib_path_from_root)
            {
                std::cout << "Given library path \"" << lib_path
                          << "\" is not from root (it should be)" << std::endl;
            }
            if(!wd_path_from_root)
            {
                std::cout << "Given working directory path \"" << lib_path
                          << "\" is not from root (it should be)" << std::endl;
            }
            return false;
        }
        bool lib_path_just_root = lib_path.length() == 1;
        wd_path_just_root = wd_path.length() == 1;
        
        const char *lib_path_p = &(lib_path[1]);
        wd_path_p = &(wd_path[1]);
        if(lib_path_just_root)
        {
            library_directory_node = &root;
        }
        else
        {
            library_directory_node = get_node_from_node(lib_path_p,
                                                        PATH_DELIMITER, 
                                                        &root, 
                                                        is_new_throwaway);
        }
    }
#endif



    if(wd_path_just_root)
    {
        working_directory_node = &root;
    }
    else
    {
        working_directory_node = get_node_from_node(wd_path_p, 
                                                    PATH_DELIMITER,
                                                    &root, 
                                                    is_new_throwaway);
    }
    return true;
}


