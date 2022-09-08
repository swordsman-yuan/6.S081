#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void 
searchFile(char* DirPath, char* FileName)
{
    char TargetDirPath[512], *Ptr;
    int Fd;
    struct dirent DirEntry;
    struct stat State;
    
    if((Fd = open(DirPath, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", DirPath);
        exit(1);
    }

    if(fstat(Fd, &State) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", DirPath);
        close(Fd);
        exit(1);
    }

    switch(State.type)
    {
        case T_FILE:
            fprintf(2, "find: you are supposed to search for a file in a directory rather than a file \n");
            break;

        case T_DIR:
            if(strlen(DirPath) + 1 + DIRSIZ + 1 > sizeof TargetDirPath)
            {
                printf("find: directory's path too long\n");
                break;
            }
            strcpy(TargetDirPath, DirPath);
            Ptr = TargetDirPath + strlen(TargetDirPath);
            *Ptr++ = '/';                                                               // construct the base path string                
            while(read(Fd, &DirEntry, sizeof(DirEntry)) == sizeof(DirEntry))
            {
                if(DirEntry.inum == 0)                                                  // skip the invalid item
                    continue;
                memmove(Ptr, DirEntry.name, DIRSIZ);
                Ptr[DIRSIZ] = 0;                                                        // contruct the abs path of file
                if(stat(TargetDirPath, &State) < 0)
                {
                    fprintf(2, "find: cannot state %s\n", TargetDirPath);
                    exit(1);
                }

                if(strcmp(FileName, DirEntry.name) == 0 && State.type == T_FILE)         // target file detected
                {
                    fprintf(1, TargetDirPath);
                    fprintf(1, "\n");
                }
                    
                else if (                                                               // search recursively, but skip . and ..
                            State.type == T_DIR &&
                            strcmp(DirEntry.name, ".") != 0 &&
                            str
                            kzcmp(DirEntry.name, "..") != 0 
                        )
                    searchFile(TargetDirPath, FileName);                                // invoke function recursively
                    
            }
            break;
    }
    close(Fd);
    return;
}

int 
main(int argc, char* argv[])
{
    if(argc < 3)
    {
        fprintf(2, "find: command needs at least 2 params\n");
        exit(1);
    }
    
    searchFile(argv[1], argv[2]);       // invoke search function
    exit(0);
}