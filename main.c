#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <stdbool.h>

#define MAX_COMMAND_ARGS 10
#define MAX_COMMAND_LENGTH 100
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

void stringToExecvArgs(char** args, char* s, int* waitFlag) {
    char* token = strtok(s, " ");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    if (strcmp(args[i - 1],"&") == 0) {
        *waitFlag = DONT_WAIT;
        args[i - 1] = NULL;
    } else {
        args[i] = NULL;
    }
}

void executeCommand(char** args, int waitFlag) {
    if (!strcmp(args[0], "exit")) {
        printf("WOW\n");
        exit(0);
    }
    pid_t pid;
    // create son process to execute the command
    pid = fork();
    if (pid < 0) {
        printf("fork error\n");
        return;
    }
    if (pid != 0) {
        printf("%d\n",pid);
    }
    // son process
    if (pid == 0) {
        int i = 0;
        if (!strcmp(args[0], "cd")) {
            int successCd = chdir(args[1]);
            if (successCd != 0) {
                fprintf(stderr, "Failed to execute %s\n", args[0]);
            }
        } else {
            execvp(args[0], args);
            // execution failed. writing to STDERR
            fprintf(stderr, "Failed to execute %s\n", args[0]);
        }
    }
    if (waitFlag && pid != 0) {
        waitpid(pid, NULL, 0);
    }
}

int main() {
    char s[MAX_COMMAND_LENGTH];
    char** args[MAX_COMMAND_ARGS];

    while (true) {
        printf("prompt> ");
        fgets(s, MAX_COMMAND_LENGTH, stdin);
        // remove new line character
        s[strlen(s) - 1] = '\0';
        int waitFlag = WAIT;
        stringToExecvArgs(args, s, &waitFlag);
        executeCommand(args, waitFlag);
    }
    return 0;
}