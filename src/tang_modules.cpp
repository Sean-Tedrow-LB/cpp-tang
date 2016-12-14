#include "tang_modules.hpp"


// TODO: get working directory

#ifdef _WIN32

#include <Windows.h>
#include <cstring>

inline bool env_to_string(const char *env_name, std::string &string_out)
{
    std::u16string wout, wenv_name;
    int env_name_length = (int)strlen(env_name);
    int wenv_name_length = ix_measure_utf8_to_utf16(env_name, env_name_length);
    wenv_name.resize(wenv_name_length);
    ix_convert_utf8_to_utf16(&(wenv_name[0]), env_name, env_name_length);
    DWORD wout_length = 0;
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

#define PATH_DELIMITER '\\'

#else

#include <cstdlib>
inline bool env_to_string(const char *env_name, std::string &string_out)
{
    const char *env = getenv(env_name);
    if(!env)
    {
        return false;
    }
    string_out = env;
    return true;
}

#define PATH_DELIMITER '/'

#endif


std::string Tang_Module_Node::get_path()
{
    // TODO
    return std::string();
}


bool Tang_Module_Tracker::initialize()
{
    // TODO
    return false;
}

Tang_Module_Node* 
Tang_Module_Tracker::load_module_from_working_dir(const std::string &path)
{
    // TODO
    return nullptr;
}
Tang_Module_Node* 
Tang_Module_Tracker::load_module_from_library_dir(const std::string &path)
{
    // TODO
    return nullptr;
}
Tang_Module_Node* 
Tang_Module_Tracker::load_module_from_module(const std::string &path, 
                                             Tang_Module_Node *source_node)
{
    // TODO
    return nullptr;
}
