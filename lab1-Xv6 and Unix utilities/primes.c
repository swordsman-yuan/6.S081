#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char* argv[])
{
    int PresentNum = 2;                 //  input number counting 
    int ParentPipeFd[2];                //  file descriptor used by pipe 
    int ChildPipeFd[2];                 //  file descriptor used by child's pipe
    // char HasNextStage = 1;              //  the pipeline has reached its ending or not


    pipe(ParentPipeFd);                 // apply for a pipe

    /* main process : write number into the pipe */
    while(PresentNum <= 35){
        write(ParentPipeFd[1], &PresentNum, 1);
        ++PresentNum;
    }

    while(1)
    {
        if(fork() == 0)                                 // child process
        {       

            close(ParentPipeFd[1]);                     // close the write side of pipe

            int Base = -1;                              // the prime child process keeps
            int ReadNumber = 0;
            int ReadCounter = 0;                        // count how many number read from pipe
            char IsPipeEstablished = 0;                 // flag indicating whether the pipeline needs to be established

            /* loop until reach the eof */
            while(read(ParentPipeFd[0], &ReadNumber, 1))
            {
                if(Base == -1)
                {
                    Base = ReadNumber;
                    fprintf(1, "prime %d\n", Base);
                }
                else if(ReadNumber % Base != 0)         // the current number has not been eliminated
                {
                    if(IsPipeEstablished == 0)          // set up pipeline only when necessary
                    {         
                        pipe(ChildPipeFd);
                        IsPipeEstablished = 1;
                        write(ChildPipeFd[1], &ReadNumber, 1);
                    }
                    else
                        write(ChildPipeFd[1], &ReadNumber, 1);
                }                                       
                ReadCounter++;                          // increment the counter
            }

            close(ParentPipeFd[0]);                     // close the read side of parent's pipe

            if(ReadCounter == 1)                        // if only 1 number has been read from the pipe, this is the last stage
                exit(0);

            ParentPipeFd[0] = ChildPipeFd[0];           // update the ParentPipeFd
            ParentPipeFd[1] = ChildPipeFd[1];                   
        }
        else
        {                                               // parent process
            close(ParentPipeFd[0]);                     // close the read end of pipe 
            close(ParentPipeFd[1]);                     // close the write end of pipe

            /* wait until all the processes exit */
            wait(0);
            exit(0);
        }
    }
    // fprintf(1, "process %d exited normally\n", getpid());
    
}