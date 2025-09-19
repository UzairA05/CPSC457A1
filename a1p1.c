/* Uzair Ansari
 * 30205691
 *
 * a1p1.c
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

    // read the whole matrix from standard input
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Error: invalid input\n");
                return 1;
            }
        }
    }

    pid_t pids[ROWS];          // keep track of child PIDs
    int treasure_row = -1;     // which row has the treasure
    pid_t treasure_pid = -1;   // PID of the child that found it

    // create 100 child processes (one per row)
    for (i = 0; i < ROWS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // child process
            printf("Child %d (PID %d): Searching row %d\n", i, getpid(), i);
            for (j = 0; j < COLS; j++) {
                if (matrix[i][j] == 1) {
                    // exit with code 1 if found
                    exit(1);
                }
            }
            // exit with code 0 if not found
            exit(0);
        } else if (pid > 0) {
            // parent stores child PID
            pids[i] = pid;
        } else {
            // fork failed
            perror("fork");
            return 1;
        }
    }

    // parent waits for all children
    for (i = 0; i < ROWS; i++) {
        int status;
        pid_t pid = wait(&status);
        // if child exited with code 1, it means treasure was found
        if (WIFEXITED(status) && WEXITSTATUS(status) == 1) {
            // find which row this PID belongs to
            for (int r = 0; r < ROWS; r++) {
                if (pids[r] == pid) {
                    treasure_row = r;
                    treasure_pid = pid;
                }
            }
        }
    }

    if (treasure_row != -1) {
        // parent goes back to matrix to figure out the column
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
