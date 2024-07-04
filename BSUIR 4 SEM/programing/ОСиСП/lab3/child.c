#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_ENV_VARS 100

void print_environment(char **envp) {
    // Print environment variables
    for (int i = 0; envp[i] != NULL; ++i) {
        printf("%s\n", envp[i]);
    }
}

int main(int argc, char *argv[], char **envp) {
    printf("\nChild process:\t");
    printf("Name: %s\t", argv[0]);
    printf("PID: %d\t", getpid());
    printf("PPID: %d\t", getppid());

    // Open file passed as command line argument
    if (argc != 2) {
        fprintf(stderr, "Usage: %s\t", argv[1]);
        exit(EXIT_FAILURE);
    }

    FILE *env_file = fopen(argv[1], "r");
    if (env_file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Print environment variables from file
    char var_name[100];
    while (fscanf(env_file, "%s", var_name) != EOF) {
        char *value = getenv(var_name);
        if (value != NULL) {
            printf("%s=%s\n", var_name, value);
        }
    }

    fclose(env_file);
    return 0;
}
