#ifndef __TANG_ARGUMENT_PARSER__
#define __TANG_ARGUMENT_PARSER__

#include <string>
#include <vector>

enum Tang_Compiler_Mode
{
    TANG_MODE_TEXT_WITHOUT_COMMENTS,
    TANG_MODE_BLOCKS_AND_STATEMENTS,
    TANG_MODE_PARTS,
    TANG_MODE_AFTER_ASYNC,
    TANG_MODE_AFTER_SYNC,
    TANG_MODE_C
};

#define TANG_FLAG_OUT                          "--out"
#define TANG_FLAG_IN                           "--in"
#define TANG_FLAG_INCLUDE                      "--include"
#define TANG_FLAG_LIB_INCLUDE                  "--lib-include"
#define TANG_FLAG_META_FLAG                    "--meta-flag"

#define TANG_FLAG_MODE_TEXT_WITHOUT_COMMENTS   "--text-without-comments"
#define TANG_FLAG_MODE_BLOCKS_AND_STATEMENTS   "--blocks-and-statements"
#define TANG_FLAG_MODE_PARTS                   "--parts"
#define TANG_FLAG_MODE_AFTER_ASYNC             "--after-async"
#define TANG_FLAG_MODE_AFTER_SYNC              "--after-sync"


struct Tang_Argument_Parser
{
    bool parse(int argc, char **argv);
    
    std::string              out_path;
    std::vector<std::string> in_paths, 
                             includes, 
                             lib_includes, 
                             meta_flags;
    Tang_Compiler_Mode       mode            = TANG_MODE_C;
    bool                     successful      = true,
                             help_requested  = false;
};

void print_help();

#endif
