#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

void 
execCommand(char* Command, char* Argv[MAXARG])
{
    if(fork() == 0)
    {
        // printf("5\n");
        exec(Command, Argv);
        // printf("6\n");
    }
    else
    {
        // printf("7\n");
        wait(0);
    }
}

int 
main(int argc, char* argv[])
{
    char CmdPath[512] = "./user/", *Ptr;
    char* Argv[MAXARG];                                     // the array is used to convey param to command
    int StartIndex = argc - 2;                              // how many arguments original command line has kept

    /* E.g: xargs grep hello */
    /* parse the original command */
    Ptr = CmdPath + strlen(CmdPath);
    memmove(Ptr, argv[1],  strlen(argv[1]));
    Ptr[strlen(argv[1])] = 0;                               // get the executable file's path
    printf("the command is %s\n", CmdPath);

    // printf("argc is %d\n", argc);
    for(int i = 0 ; i < argc ; ++i)
        printf("argv[%d] is %s\n", i, argv[i]);

    for(int i = 2 ; i < argc ; ++i)                         // copy the orginal param to Argv
    {
        Ptr = Argv[i - 2] + strlen(Argv[i - 2]);

        memmove(Ptr, argv[i], strlen(argv[i])); 
        Ptr[strlen(argv[i])] = 0;
    }
    
    printf("Argv[0] is %s\n", Argv[0]);

    // printf("2\n");

    char ReadChar;
    Ptr = Argv[StartIndex];                                 // move Ptr to the begining of new param 
    while(read(0, &ReadChar, 1))                            // if not reach the EOF
    {
        if(ReadChar != '\n')
        {
            // printf("3\n");
            *Ptr = ReadChar;
            ++Ptr;
        }
        else
        {
            // printf("4\n");
            *Ptr = 0;                                       // set the terminal flag
            for(int i = 0 ; i <= StartIndex ; ++i)
                printf("the current param passed by xargs is %s\n", Argv[i]);
            execCommand(CmdPath, Argv);     
            Ptr = Argv[StartIndex];                         // reset Ptr to the begining of new param
        }
    }

    exit(0);
}