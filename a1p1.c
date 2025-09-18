#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define ROWS 100
#define COLS 1000

int main() {
    int matrix[ROWS][COLS];
    int i, j;

    // read the matrix
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Error reading input\n");
                return 1;
            }
        }
    }

    pid_t child_pids[ROWS];

    int treasure_row = -1;
    pid_t treasure_pid = -1;

    for (i = 0; i < ROWS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // child
            printf("Child %d (PID %d): Searching row %d\n", i, getpid(), i);
            for (j = 0; j < COLS; j++) {
                if (matrix[i][j] == 1) {
                    exit(1); // signal found
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
            // identify row of this PID
            for (int r = 0; r < ROWS; r++) {
                if (child_pids[r] == pid) {
                    treasure_row = r;
                    treasure_pid = pid;
                    break;
                }
            }
        }
    }

    if (treasure_row != -1) {
        // re-scan to get the column
        int col_found = -1;
        for (j = 0; j < COLS; j++) {
            if (matrix[treasure_row][j] == 1) {
                col_found = j;
                break;
            }
        }
        printf("Parent: The treasure was found by child with PID %d at row %d and column %d\n",
               treasure_pid, treasure_row, col_found);
    } else {
        printf("Parent: No treasure found\n");
    }

    return 0;
}
