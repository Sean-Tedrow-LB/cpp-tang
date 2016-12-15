#include "tang_file_reader.hpp"
#include "tang_comment_removal.hpp"
#include "tang_argument_parser.hpp"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <vector>
#include "tang_file_writer.hpp"
#include "tang_modules.hpp"


// TODO: make writer and reader take std::string for open()


#ifdef _WIN32
#include "ix_unicode.hpp"

#define GET_ARGV       &(argv[0])

int wmain(int argc, wchar_t **argvw)
{
    std::vector<std::string> argv_strings;
    std::vector<char*> argv;
    
    argv_strings.resize(argc);
    argv.resize(argc);
    
    for(int i = 0; i < argc; i++)
    {
        std::string &arg = argv_strings[i];
        const wchar_t *argw = argvw[i];
        int argw_length = (int)wcslen(argw);
        int arg_length = ix_measure_utf16_to_utf8((const char16_t*)argw, argw_length);
        arg.resize(arg_length);
        char *argp = &(arg[0]);
        ix_convert_utf16_to_utf8(argp, (const char16_t*)argw, argw_length);
        argv[i] = argp;
    }

#else

#define GET_ARGV        argv

int main(int argc, char **argv)
{
#endif
    // TODO: check for --help or -h
    Tang_Argument_Parser arg_parser;
    if(!arg_parser.parse(argc, GET_ARGV))
    {
        return -1;
    }
    if(arg_parser.mode != TANG_MODE_TEXT_WITHOUT_COMMENTS)
    {
        std::cout << "In this early build of the compiler,"
                     " the output mode must be set to "
                     TANG_FLAG_MODE_TEXT_WITHOUT_COMMENTS << std::endl;
        return -1;
    }
    
    // TODO use Tang_Module_Tracker
    
    Tang_Module_Tracker module_tracker;
    if(!module_tracker.initialize())
    {
        return -1;
    }
    for(int i = 0; i < (int)arg_parser.in_paths.size(); i++)
    {
        const std::string &in_path = arg_parser.in_paths[i];
        module_tracker.load_module_from_working_dir(in_path);
    }
    Tang_File_Writer writer;
    if(!writer.open(arg_parser.out_path))
    {
        return -1;
    }
    std::string header;
    for(int i = 0; i < (int)module_tracker.modules.size(); i++)
    {
        Tang_Module_Node *module = module_tracker.modules[i];
        header = std::string("\n\n\n***** FOLLOWING IS TEXT OF MODULE \"") + 
                                              module->name + "\" *****\n\n\n";
        writer.write(header);
        writer.write(module->text.text);
    }
    return 0;
}
