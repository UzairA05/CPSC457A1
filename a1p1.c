#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define ROWS 100
#define COLS 1000

int main() {
    int matrix[ROWS][COLS];
    int i, j;

    // read matrix from stdin
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Error reading input\n");
                return 1;
            }
        }
    }

    pid_t child_pids[ROWS];

    for (i = 0; i < ROWS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // child
            printf("Child %d (PID %d): Searching row %d\n", i, getpid(), i);
            for (j = 0; j < COLS; j++) {
                if (matrix[i][j] == 1) {
                    // found treasure
                    exit(1);
                }
            }
            exit(0);
        } else if (pid > 0) {
            child_pids[i] = pid;
        } else {
            perror("fork");
            exit(1);
        }
    }

    // parent waits
    for (i = 0; i < ROWS; i++) {
        int status;
        pid_t pid = wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 1) {
            printf("Parent: Child with PID %d says it found treasure!\n", pid);
        }
    }

    return 0;
}
