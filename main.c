#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_COMMAND_ARGS 10

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
    int numOfWords = getNumberOfWords(str);
    char** args = (char**) malloc((numOfWords + 1) * sizeof(char*));
    if (args == NULL) {
        printf("Failed to execute command due to memory problem\n");
        exit(1);
    }
    char* token = strtok(str, " ");
    int i = 0;
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    return args;
}

int main() {
    char s[] = "/bin/ls -r -t -l";
    char** args = stringToExecvArgs(s);
    printf("%s %s %s\n", args[0], args[1], args[2]);
/*    printf("Enter any key to end\n");
    char end;
    scanf(" %c", &end);*/
    execv(args[0], args);
    free(args);
    return 0;
}