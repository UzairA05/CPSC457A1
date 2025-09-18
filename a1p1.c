/* a1p1.c - Part 1 starter
 * Compile: gcc -O2 -Wall a1p1.c -o a1p1
 * Run: ./a1p1 < inputfilep1.txt
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

    // read the matrix
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            if (scanf("%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "Error reading input\n");
                return 1;
            }
        }
    }

    // fork children, one per row
    for (i = 0; i < ROWS; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // child process
            printf("Child %d (PID %d): Searching row %d\n", i, getpid(), i);
            // TODO: search row and report result
            exit(0);
        }
    }

    // parent waits for children
    for (i = 0; i < ROWS; i++) {
        wait(NULL);
    }

    printf("Parent: waiting done\n");
    return 0;
}
