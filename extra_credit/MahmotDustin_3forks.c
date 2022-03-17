
//Dustin Mahmot 
//Larson 421 fork assignment
//Feb 22

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    printf("FORK 1 from %d\n", getpid());
    pid_t id = fork();

    if (id < 0){
        printf("fork failed.\n");
        return 1;
    }

    else if(id == 0){ //child
        printf("my pid: %d, parent pid: %d\n", getpid(), getppid());
    }

    else {//parent
        printf("my pid: %d, waiting for %d\n", getpid(), id);
        wait(NULL);
        printf("%d is done waiting for %d.\n", getpid(), id);
    }

    printf("FORK 2 from %d\n", getpid());

    pid_t id2 = fork();

    if(id2 < 0){
        printf("fork2 failed.\n");
        return 2;
    }

    else if (id2 == 0){//child
        printf("my pid: %d, parent pid: %d\n", getpid(), getppid());
    }

    else {//parent
        printf("my pid: %d, waiting for %d\n", getpid(), id2);
        wait(NULL);
        printf("%d is done waiting for %d.\n", getpid(), id2);

    }

    printf("FORK 3 from %d\n", getpid());
    
    pid_t id3 = fork();

    if(id3 < 0){
        printf("fork3 failed.\n");
        return 3;
    }

    else if (id3 == 0){//child
        printf("my pid: %d, parent pid: %d\n", getpid(), getppid());
    }

    else {//parent
        printf("my pid: %d, waiting for %d\n", getpid(), id3);
        wait(NULL);
        printf("%d is done waiting for %d.\n", getpid(), id3);

    }
    
    return 0;
}