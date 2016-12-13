#include "tang_argument_parser.hpp"
#include <cstring>
#include <iostream>


typedef bool (*Tang_Binary_Set_Func)(const char *value, 
                                     Tang_Argument_Parser &parser);

struct Tang_Binary_Flag
{
    const char *flag;
    Tang_Binary_Set_Func output_func;
};

struct Tang_Binary_Parser
{
    std::vector<Tang_Binary_Flag> flags;
    void add_flag(const char *flag, Tang_Binary_Set_Func output_func)
    {
        int i = flags.size();
        flags.resize(i + 1);
        Tang_Binary_Flag &f = flags[i];
        f.flag = flag;
        f.output_func = output_func;
    }
    bool search_for_flags(const char *flag, const char *value,
                          Tang_Argument_Parser &parser)
    {
        for(int i = 0; i < (int)flags.size(); i++)
        {
            Tang_Binary_Flag &binary_flag = flags[i];
            if(!strcmp(binary_flag.flag, flag))
            {
                bool result = binary_flag.output_func(value, parser);
                if(!result)
                {
                    parser.successful = false;
                }
                return true;
            }
        }
        return false;
    }
};


static bool set_out(const char *value, Tang_Argument_Parser &parser)
{
    bool result = parser.out_path.empty();
    if(result)
    {
        parser.out_path = value;
    }
    else
    {
        std::cout << "The " << TANG_FLAG_OUT << " flag can only be given once" << std::endl;
    }
    return result;
}

inline void append(std::vector<std::string> &list, const char *value)
{
    int i = list.size();
    list.resize(i + 1);
    list[i] = value;
}

static bool set_in(const char *value, Tang_Argument_Parser &parser)
{
    append(parser.in_paths, value);
    return true;
}

static bool set_include(const char *value, Tang_Argument_Parser &parser)
{
    append(parser.includes, value);
    return true;
}

static bool set_lib_include(const char *value, Tang_Argument_Parser &parser)
{
    append(parser.lib_includes, value);
    return true;
}

static bool set_meta_flag(const char *value, Tang_Argument_Parser &parser)
{
    append(parser.meta_flags, value);
    return true;
}

struct Tang_Mode_Flag
{
    const char *flag;
    Tang_Compiler_Mode mode;
};

struct Tang_Mode_Parser
{
    std::vector<Tang_Mode_Flag> flags;
    
    void add_flag(const char *flag, Tang_Compiler_Mode mode)
    {
        Tang_Mode_Flag f = {flag, mode};
        flags.push_back(f);
    }
    
    bool search_for_flags(const char *flag, Tang_Argument_Parser &parser)
    {
        for(int i = 0; i < (int)flags.size(); i++)
        {
            Tang_Mode_Flag &f = flags[i];
            if(!strcmp(flag, f.flag))
            {
                if(parser.mode == TANG_MODE_C)
                {
                    parser.mode = f.mode;
                }
                else
                {
                    parser.successful = false;
                    std::cout << "The output mode can only be set once" << std::endl;
                }
                return true;
            }
        }
        return false;
    }
};

bool Tang_Argument_Parser::parse(int argc, char **argv)
{
    Tang_Binary_Parser binary_parser;
    binary_parser.add_flag(TANG_FLAG_OUT, set_out);
    binary_parser.add_flag(TANG_FLAG_IN, set_in);
    binary_parser.add_flag(TANG_FLAG_INCLUDE, set_include);
    binary_parser.add_flag(TANG_FLAG_LIB_INCLUDE, set_lib_include);
    binary_parser.add_flag(TANG_FLAG_META_FLAG, set_meta_flag);
    
    Tang_Mode_Parser mode_parser;
    mode_parser.add_flag(TANG_FLAG_MODE_TEXT_WITHOUT_COMMENTS, 
                         TANG_MODE_TEXT_WITHOUT_COMMENTS);
    mode_parser.add_flag(TANG_FLAG_MODE_BLOCKS_AND_STATEMENTS, 
                         TANG_MODE_BLOCKS_AND_STATEMENTS);
    mode_parser.add_flag(TANG_FLAG_MODE_AFTER_ASYNC, 
                         TANG_MODE_AFTER_ASYNC);
    mode_parser.add_flag(TANG_FLAG_MODE_AFTER_SYNC, 
                         TANG_MODE_AFTER_SYNC);
    
    for(int i = 1; i < argc; i++)
    {
        bool result = false;
        const char *flag = argv[i];
        if((i + 1) < argc)
        {
            const char *value = argv[i + 1];
            result = binary_parser.search_for_flags(flag, value, *this);
            if(result)
            {
                i++;
            }
        }
        if(!result)
        {
            if(!mode_parser.search_for_flags(flag, *this))
            {
                std::cout << "Unrecognized flag: " << flag << std::endl;
                successful = false;
            }
        }
    }
    return successful;
}
