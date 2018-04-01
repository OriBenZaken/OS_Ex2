#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

#define MAX_COMMAND_ARGS 10
#define MAX_COMMAND_LENGTH 50
#define DONT_WAIT 0
#define WAIT 1

int getNumberOfWords(char* str) {
    int count = 0;
    while(*str != '\0') {
        if (*str == ' ') {
            count++;
        }
        str++;
    }
    return count + 1;
}

char** stringToExecvArgs(char* str) {
/*    int numOfWords = getNumberOfWords(str);
    char** args = (char**) malloc((numOfWords + 1) * sizeof(char*));
    if (args == NULL) {
        printf("Failed to execute command due to memory problem\n");
        exit(1);
    }*/
    char** args[MAX_COMMAND_ARGS];
    char* token = strtok(str, " ");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    return args;
}

void executeCommand(char** args, int waitFlag) {
    pid_t pid;
    // create son process to execute the command
    pid = fork();
    if (pid != 0) {
        printf("%d\n",pid);
    }
    // son process
    if (pid == 0) {
        printf("bitch\n");

        int i = 0;
        while(args[i] != NULL) {
            printf("%s\n", args[i]);
            i++;
        }
        execv(args[0], args);
    }
    if (waitFlag && pid != 0) {
        waitpid(pid, NULL, WCONTINUED);
    }
}

int main() {
    char s[MAX_COMMAND_LENGTH];
    printf("prompt> ");
    fgets(s, MAX_COMMAND_LENGTH, stdin);
    // remove new line character
    s[strlen(s) - 1] = '\0';
    char** args[MAX_COMMAND_ARGS];
    int waitFlag = WAIT;
    char* token = strtok(s, " ");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    if (strcmp(args[i - 1],"&") == 0) {
        waitFlag = DONT_WAIT;
        args[i - 1] = NULL;
    } else {
        args[i] = NULL;
    }
    executeCommand(args, waitFlag);
    return 0;
}