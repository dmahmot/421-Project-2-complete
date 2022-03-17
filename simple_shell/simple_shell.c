#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include "utils.h"


//any leading and trailing spaces will be removed
char* stripEndSpaces(char* input){
    
    while(isspace(*input)){
        input++;
    }
    
    while(isspace(input[strlen(input)-1])){
        input[strlen(input)-1] = 0;
    }

    char* strippedInput = malloc((strlen(input) * sizeof(char))+1);
    if(!strippedInput){
        fprintf(stderr, "%s\n", strerror(errno));
        exit(errno);
    }

    strcpy(strippedInput, input); //input is freed in main

    return strippedInput;
}

//accpets a char[] (user input) and returns the number of arguments (including the command)
//ex: "ls -a" returns 2
int getNumArgs(char* input){

    int numElems = 0;
    
    int n = 0;
    while(input[n]){
        while(!isspace(input[n]) && input[n]){
            n++;
        }
        
        while(isspace(input[n])){
            n++;
        }

        numElems++;
    }
    return numElems;
}


// takes user input, splits the commands and arguments into array elements
char** getArgArray(char* input, int numArgs){

    //args is a an array containing char*'s.
    //each element will be a char* (char[]) - an argument
    char** args = (char**)malloc((numArgs+1) * sizeof(char*)); //1 extra arg for NULL terminator
    if(!args) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(errno);
    }


    int inputPos = 0; //the index in relation to the entire input

    for(int i=0; i<numArgs; i++){//loop once for each arg
       
        //ignore whitepace before arg
        while(isspace(input[inputPos])){
            inputPos++;
        }

        int argStart = inputPos;

        //find end of arg
        while(!isspace(input[inputPos]) && input[inputPos]){
            inputPos++;
        }

        int argLength = inputPos - argStart;
        
        //allocate mem for arg string
        args[i] = (char*)malloc(argLength+1 * sizeof(char));
        if(!args[i]){
            fprintf(stderr, "%s\n", strerror(errno));
            exit(errno);
        }

        //build the string
        for(int j=0; j<argLength;j++){
            args[i][j] = input[argStart+j];
        }
        //add null terminator to arg string
        args[i][argLength] = 0;
    }   
    
    //add NULL terminator to arg array (for exec function)
    args[numArgs] = NULL;
    return args;
}

void destroyStuff(char* input, char** args, int numArgs){
    free(input);

    for(int i=0; i<numArgs+1; i++){
        free(args[i]);
    }

    free(args);
}

int main(int argc, char *argv[]){

    //check command line args    
    if (argc > 1){
        fprintf(stderr, "Invalid arguments: ");
        for(int i=1; i<argc; i++){
            fprintf(stderr, "%s ", argv[i]);
        }
        fprintf(stderr, "\nsimple_shell does not accept any command line arguments.\n");
        return 1;
    }

    //main loop
    while(1){
        printf("$ ");

        int numChars = 32; //some initial size - getline will realloc anyway
        size_t buffersize = numChars * sizeof(char);

        //pointer to user input
        char *input = (char*)malloc(buffersize);
        if(!input) {
            fprintf(stderr, "%s\n", strerror(errno));
            exit(errno);
        }
        
        //get user input
        getline(&input, &buffersize, stdin);

        //remove any trailing whitespace
        char* sInput = stripEndSpaces(input);
        free(input);

        //unescape input
        char* strippedInput = unescape(sInput, stderr);
        free(sInput);

        //get num args
        int numArgs = getNumArgs(strippedInput);

        //separate args into an array
        char** args = getArgArray(strippedInput, numArgs);

       
        //EXIT CASE
        if (strcmp(args[0], "exit") == 0){
            //return 0
            if(numArgs == 1){
                destroyStuff(strippedInput,args,numArgs);
                return 0;
            }

            //exit code was given (possibly) - try to parse as int
            else if (numArgs == 2){

                //parse 2nd int to check for non integer
                int n=0;
                while(args[1][n]){
                    if(!isdigit(args[1][n])){
                        if(n==0 && args[1][n] == '-'){ /*do nothing, first input was '-'*/}

                        else{//non int input
                            n = -1;
                            break;
                        }
                    }  
                    n++;
                }
                
                if(n < 0){ //input was not an integer, ignore and remprompt user with $
                    destroyStuff(strippedInput,args,numArgs);
                    continue;
                }
                else{ //input WAS int, exit using that int as exit status
                    int val = atoi(args[1]);
                    destroyStuff(strippedInput,args,numArgs);
                    return val;
                }
                
            }

            //too many args, reprompt user
            else{
                destroyStuff(strippedInput,args,numArgs);
                continue;
            }

        }


        //PROC command CASE
        else if(strcmp(args[0], "proc") == 0){

            //check for too many args with proc
            if(numArgs > 2){
                fprintf(stderr, "Too many arguments with proc.\n");
                destroyStuff(strippedInput, args, numArgs);
                continue;
            }
            else if (numArgs == 1){ //'proc' was entered. do nothing, reprompt user
                destroyStuff(strippedInput, args, numArgs);
                continue;
            }
            
            int lenP;

            if(args[1]){
                lenP = (int)strlen(args[1]);
            }
            else{
                lenP = 0;
            }

            //allocate mem: 6 (for /proc/), enough for rest of filepath, then one for null term
            char* filepath = (char*)malloc((6 + lenP + 1) * sizeof(char));
            if(!filepath){
                fprintf(stderr, "%s\n", strerror(errno));
                exit(errno);
            }
            
            //add /proc/ to filepath
            filepath[0] = '/';
            for(int i=1; i<5; i++){
                filepath[i] = args[0][i-1];
            }
            filepath[5] = '/';
            
            
            //add arg 1 to path
            for(int i=6; i < (6+lenP); i++){
                filepath[i] = args[1][i-6];
            }
            
            //add null terminator
            filepath[(6+lenP)] = 0;
            
                        
            FILE *file = fopen(filepath, "r");
            
            if(file){

                char c;
                while((c = fgetc(file)) !=EOF){
                    printf("%c", c);
                }

                //print newline to comply with rubric
                printf("\n");

                //cleanup
                destroyStuff(strippedInput, args, numArgs);
                free(filepath);
                fclose(file);
            }
            else{
                fprintf(stderr, "the file %s could not be opened.\n", filepath);
                free(filepath);
                destroyStuff(strippedInput, args, numArgs);
            }
        }

        //NON-PROC, NON-EXIT CASE
        else{
            pid_t id = fork();
            if(id < 0){
                fprintf(stderr, "%s\n", strerror(errno));
            }
            else if (id == 0){ 
                if(execvp(args[0], args) == -1){
                    fprintf(stderr, "%s\n", strerror(errno));
                    destroyStuff(strippedInput, args, numArgs);
                    return 0;
                }
            }
            else{ 
                wait(NULL);
                destroyStuff(strippedInput, args, numArgs);
                // printf("parent finished\n");
            }
        }        
    }
    


    return 0;
}