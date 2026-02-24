/*
Made By: Thien Nguyen
Project Description: A simple shell in C
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
//#include <sys/wait.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

int main() {
    char input[MAX_INPUT + 1]; //max input command
    char *args[MAX_ARGS]; //max amount of arguments

    while (1) {
        printf("mini-bash>");
        fflush(stdout); //flush output buffer then ask for input
        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            perror("fgets failed");
            continue;
        }

        input[strcspn(input, "\r\n")] = '\0'; //removes '\n'

        if (strcmp(input, "exit") == 0) {
            printf("mini-bash successfully exited.");
            break;
        }

        char *token = strtok(input, " "); //tokens the input into args

        int i = 0;
        while (token != NULL && i < MAX_ARGS) {
            args[i] = token;
            token = strtok(NULL, " "); //continues tokening after first arg
            i++;
        }
        args[i] = NULL;

        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                perror("cd missing arguments");
            }else if (chdir(args[1]) != 0) {
                perror("cd failed");
            }
            continue;
        }

        pid_t pid = fork(); //forks OS and returns parent and child OS 

        if (pid < 0) { //fork failed if pid is -1
            perror("fork failed");
        }else if (pid == 0) { //child system
            execvp(args[0], args);
            perror("execvp failed"); //if execvp returns, then it failed
            exit(EXIT_FAILURE);
        }else { //parent system
            int status;
            waitpid(pid, &status, 0);
            printf("Exit Status: %d\n", status);
        }
        //printf("%s\n", input); //test to see if stdin is working
        
    }
    return 0;
}