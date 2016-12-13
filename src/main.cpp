#include "tang_file_reader.hpp"
#include "tang_comment_removal.hpp"
#include "tang_argument_parser.hpp"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cerrno>

int main(int argc, char **argv)
{
    // TODO: check for --help or -h
    Tang_Argument_Parser arg_parser;
    if(!arg_parser.parse(argc, argv))
    {
        return -1;
    }
    if(arg_parser.out_path.empty())
    {
        std::cout << "Output path not provided" << std::endl;
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
    Tang_File_Reader reader;
    if(!reader.open(arg_parser.in_paths[0].c_str()))
    {
        return -1;
    }
    Tang_Text_Without_Comments text;
    text.from(reader);
    FILE *f = fopen(arg_parser.out_path.c_str(), "w");
    if(!f)
    {
        std::cout << "Failed to open file \"" << arg_parser.out_path.c_str();
        std::cout << "\" with error \"" << std::strerror(errno) << "\"\n";
        return -1;
    }
    fwrite(text.text.data(), sizeof(char), text.text.length(), f);
    fclose(f);
    return 0;
}
