#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_ENV_VARS 100

extern char **environ;

int compare_strings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void print_sorted_environment() {
    char *env[MAX_ENV_VARS];
    int i = 0;

    // Copy the environment variables
    for (char **envp = environ; *envp != NULL && i < MAX_ENV_VARS; ++envp) {
        env[i++] = *envp;
    }

    // Sort the copied environment variables
    qsort(env, i, sizeof(char *), compare_strings);

    // Print the sorted environment variables
    for (int j = 0; j < i; ++j) {
        printf("%s\n", env[j]);
    }
}

int main(int argc, char *argv[]) {
    // Check if the environment variable file is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <env_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open the environment variable file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // Read environment variables from the file
    char *envp[MAX_ENV_VARS];
    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file) != NULL && count < MAX_ENV_VARS) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        envp[count++] = strdup(line);
    }
    fclose(file);

    // Set the environment for the child process
    if (clearenv() != 0) {
        perror("clearenv");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < count; ++i) {
        if (putenv(envp[i]) != 0) {
            perror("putenv");
            exit(EXIT_FAILURE);
        }
    }

    // Print CHILD_PATH if available
    for (int i = 0; i < count; ++i) {
        if (strncmp(envp[i], "CHILD_PATH=", strlen("CHILD_PATH=")) == 0) {
            printf("CHILD_PATH: %s\n", envp[i] + strlen("CHILD_PATH="));
            break;
        }
    }

    // Counter for child process names
    int child_count = 0;

    // Main loop for keyboard input
    char input;
    while (1) {
        printf("Enter a command (+, *, &, q): ");
        scanf(" %c", &input);

        switch (input) {
            case '+':
            case '*':
            case '&': {
                // Fork and execute child process
                pid_t pid = fork();
                if (pid == -1) {
                    perror("fork");
                    exit(EXIT_FAILURE);
                } else if (pid == 0) {
                    // Child process
                    char child_name[20];
                    snprintf(child_name, sizeof(child_name), "child_%02d", child_count++);

                    char *args[] = {child_name, NULL};
                    if (execve("./child", args, environ) == -1) {
                        perror("execve");
                        exit(EXIT_FAILURE);
                    }
                }
                break;
            }
            case 'q':
                // Terminate the parent process
                exit(EXIT_SUCCESS);
            default:
                printf("Invalid command\n");
        }
    }

    return 0;
}
