//Dustin Mahmot 
//Larson 421 fork assignment (experiment)
//Feb 2022

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main()
{

    printf("main process is pid: %d\n", getpid());

    

    pid_t id = fork();

    if (id < 0){
        printf("fork failed.\n");
        return 1;
    }
    else if(id == 0){
        //Child process
        printf(" %d with parent %d\n", getpid(), getppid());
        printf(" sleeping for 3 sec...\n", getpid());
        sleep(3); //sleeps for 3 seconds (parent process will terminate first)
        printf(" after sleeping, %d with parent %d\n", getpid(), getppid());
    }
    else {
        //parent process
        printf("in %d - parent is %d\n", getpid(), getppid());
        sleep(2); //sleeps for 2 seconds (will terminate before child process)
    }

    


   

   
    // printf("FORK 2 from %d\n", getpid());

    // pid_t id2 = fork();

    // if(id2 < 0){
    //     printf("fork2 failed.\n");
    //     return 2;
    // }

    // else if (id == 0){
    //     printf("in %d, child of %d\n", getpid(), getppid());
    // }

    // printf("FORK 3 from %d\n", getpid());
    
    // pid_t id3 = fork();

    // if(id3 < 0){
    //     printf("fork3 failed.\n");
    //     return 3;
    // }

    // else if (id == 0){
    //     printf("in %d, child of %d\n", getpid(), getppid());
    // }
    

    return 0;
}