#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

void 
execCommand(char* Command, char* Argv[MAXARG])
{
    if(fork() == 0)
        exec(Command, Argv);
    else
        wait(0);
    
}

int 
main(int argc, char* argv[])
{
    char CmdPath[512] = "", *Ptr;
    char* Argv[MAXARG];                                     // the array is used to convey param to command
    int StartIndex = argc - 1;                              // how many arguments original command line has kept

    /* E.g: xargs grep hello */
    /* parse the original command */
    Ptr = CmdPath + strlen(CmdPath);
    memmove(Ptr, argv[1],  strlen(argv[1]));
    Ptr[strlen(argv[1])] = 0;                               // get the executable file's path

    for(int i = 1 ; i < argc ; ++i)                         // copy the orginal param to Argv
    {
        uint Length = strlen(argv[i]) + 1;
        Argv[i - 1] = malloc(Length); 
        memmove(Argv[i - 1], argv[i], Length); 
    }
    Argv[StartIndex + 1] = 0;                               // set the next element of Argv as NULL

    char ReadChar;
    Argv[StartIndex] = malloc(512);                         // allocate a 512 bytes space for additional params
    Ptr = Argv[StartIndex];                                 // move Ptr to the begining of new param                         
    while(read(0, &ReadChar, 1))                            // if not reach the EOF
    {
        if(ReadChar != '\n')
        {
            *Ptr = ReadChar;
            ++Ptr;
        }
        else
        {
            *Ptr = 0;                                       // set the terminal flag
            execCommand(CmdPath, Argv);     
            Ptr = Argv[StartIndex];                         // reset Ptr to the begining of new param
        }
    }

    for(int i = 0 ; i <= StartIndex ; ++i)                  // release the memory
        free(Argv[i]);

    exit(0);
}