#ifndef __TANG_MODULE_RAW__
#define __TANG_MODULE_RAW__
#include "tang_comment_removal.hpp"

void Tang_Text_Without_Comments::from(Tang_File_Reader &source)
{
    do
    {
        char c = source.get();
        if(c == '/')
        {
            if(!source.next())
            {
                return;
            }
            c = source.get();
            if(c != '*')
            {
                text += '/';
            }
            else
            {
                if(!source.next())
                {
                    return;
                }
                c = source.get();
                int comment_depth = 1;
                do
                {
                    if(c == '/')
                    {
                        if(!source.next())
                        {
                            return;
                        }
                        c = source.get();
                        if(c == '*')
                        {
                            comment_depth++;
                        }
                    }
                    else if(c == '*')
                    {
                        if(!source.next())
                        {
                            return;
                        }
                        c = source.get();
                        if(c == '/')
                        {
                            comment_depth--;
                        }
                    }
                    if(c == '\n')
                    {
                        text += '\n';
                        if(!source.next())
                        {
                            return;
                        }
                        c = source.get();
                    }
                    else if(c == '\r')
                    {
                        text += '\n';
                        if(!source.next())
                        {
                            return;
                        }
                        c = source.get();
                        if(c == '\n')
                        {
                            if(!source.next())
                            {
                                return;
                            }
                        }
                    }
                    else
                    {
                        if(!source.next())
                        {
                            return;
                        }
                        c = source.get();
                    }
                }
                while(comment_depth);
            }
        }
        text += c;
    }
    while(source.next());
}

#endif
