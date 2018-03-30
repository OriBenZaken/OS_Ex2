#include <stdio.h>
#include <string.h>

int main() {
    char s[] = "ls -l here";
    char* token = strtok(s, " ");
    int i = 0;
    while (token != NULL) {
        printf("%s\n", token);
        token = strtok(NULL, " ");
    }
    printf("Enter any key to end\n");
    char end;
    scanf(" %c", &end);
    return 0;
}