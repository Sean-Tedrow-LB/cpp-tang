#include "tang_file_reader.hpp"
#include "tang_comment_removal.hpp"
#include "tang_argument_parser.hpp"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <vector>
#include "tang_file_writer.hpp"

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
        int arg_length = ix_measure_utf16_to_utf8(argw, argw_length);
        arg.resize(arg_length);
        char *argp = &(arg[0]);
        ix_convert_utf16_to_utf8(argp, argw, argw_length);
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
    // TODO: temporarily limiting to one input file
    if(arg_parser.in_paths.size() != 1)
    {
        std::cout << "In this early build of the compiler," 
                     " precisely one input file must be provided" << std::endl;
        return -1;
    }
    if(arg_parser.mode != TANG_MODE_TEXT_WITHOUT_COMMENTS)
    {
        std::cout << "In this early build of the compiler,"
                     " the output mode must be set to " <<
                     TANG_FLAG_MODE_TEXT_WITHOUT_COMMENTS << std::endl;
        return -1;
    }
    // TODO: I'm eventually going to need to keep track of module locations.
    //       This is a shortcut for now.
    Tang_Text_Without_Comments text;
    {
        Tang_File_Reader reader;
        if(!reader.open(arg_parser.in_paths[0].c_str()))
        {
            return -1;
        }
        text.from(reader);
    }
    {
        Tang_File_Writer writer;
        if(!writer.open(arg_parser.out_path.c_str()))
        {
            return -1;
        }
        writer.write(text.text);
    }
    return 0;
}
