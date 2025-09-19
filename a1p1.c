/* a1p1.c
 * - reads a 100 x 1000 matrix
 * - forks 100 children
 * - each child is assigned a row to search for the treasure
 * - parent waits for all children then determines which child (PID) found treasure
 * - parent then inspects the matrix row to find the exact column and prints row/col
 */

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
                fprintf(stderr, "Error: invalid input\n");
                return 1;
            }
        }
    }

    pid_t pids[ROWS];
    int treasure_row = -1;
    pid_t treasure_pid = -1;

    // fork children
    for (i = 0; i < ROWS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // child
            printf("Child %d (PID %d): Searching row %d\n", i, getpid(), i);
            for (j = 0; j < COLS; j++) {
                if (matrix[i][j] == 1) {
                    exit(1); // found
                }
            }
            exit(0); // not found
        } else if (pid > 0) {
            pids[i] = pid;
        } else {
            perror("fork");
            return 1;
        }
    }

    // parent waits for results
    for (i = 0; i < ROWS; i++) {
        int status;
        pid_t pid = wait(&status);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 1) {
            for (int r = 0; r < ROWS; r++) {
                if (pids[r] == pid) {
                    treasure_row = r;
                    treasure_pid = pid;
                }
            }
        }
    }

    if (treasure_row != -1) {
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
        printf("Parent: No treasure found in the matrix\n");
    }

    return 0;
}
