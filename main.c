#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <stdbool.h>

#define MAX_COMMAND_ARGS 10
#define MAX_COMMAND_LENGTH 100
#define MAX_JOBS_NUMBER 50
#define DONT_WAIT 0
#define WAIT 1

typedef struct job {
    char command[MAX_COMMAND_LENGTH];
    pid_t pid;
} job;

void initalizeJobsArray(job* jobs) {
    int i;
    for (i = 0; i < MAX_JOBS_NUMBER; i++) {
        strcpy(jobs[i].command, "");
        jobs[i].pid = -1;
    }
}

int insertToJobsArray(job* jobs, pid_t pid, char* command) {
    int i = 0;
    while(i < MAX_JOBS_NUMBER) {
        if(jobs[i].pid == -1) {
            jobs[i].pid = pid;
            strcpy(jobs[i].command, command);
            break;
        }
        i++;
    }
    if (i == MAX_JOBS_NUMBER) {
        return 0;
    }
    return 1;
}

void stringToExecvArgs(char** args, char* command, int* waitFlag) {
    char* token = strtok(command, " ");
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

void changeDirectory(char** args) {
    // command is: cd. Go to home directory
    if (args[1] ==  NULL) {
        chdir(getenv("HOME"));
        return;
    }
    int successCd = chdir(args[1]);
    if (successCd != 0) {
        fprintf(stderr, "Failed to execute %s\n", args[0]);
    }
}

void executeCommand(job* jobs, char* command, char** args, int waitFlag) {
    if (!strcmp(args[0], "exit")) {
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
            changeDirectory(args);
        } else {
            execvp(args[0], args);
            // execution failed. writing to STDERR
            fprintf(stderr, "Failed to execute %s\n", args[0]);
        }
        // main process
    } else {
        if (waitFlag) {
            waitpid(pid, NULL, 0);
        } else {
            int jobInsertSuccess = insertToJobsArray(jobs, pid, command);
        }
    }
}

int main() {

    char command[MAX_COMMAND_LENGTH];
    char** args[MAX_COMMAND_ARGS];
    job jobs[MAX_JOBS_NUMBER];
    initalizeJobsArray(jobs);

    while (true) {
        printf("prompt> ");
        fgets(command, MAX_COMMAND_LENGTH, stdin);
        // remove new line character
        command[strlen(command) - 1] = '\0';
        char commandCpy[MAX_COMMAND_LENGTH];
        strcpy(commandCpy, command);

        int waitFlag = WAIT;
        stringToExecvArgs(args, command, &waitFlag);
        executeCommand(jobs, commandCpy, args, waitFlag);
        if (!strcmp(args[0], "cat")) {
            printf("\n");
        }
    }
    return 0;
}