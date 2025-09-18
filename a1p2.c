/* a1p2.c - Part 2 starter
 * Compile: gcc -O2 -Wall a1p2.c -o a1p2 -lm
 * Run: ./a1p2 <LOWER> <UPPER> <NPROCS>
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

// prime checker function
int is_prime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s LOWER UPPER NPROCS\n", argv[0]);
        return 1;
    }

    int lower = atoi(argv[1]);
    int upper = atoi(argv[2]);
    int nprocs = atoi(argv[3]);

    int total = upper - lower + 1;
    if (total <= 0 || nprocs <= 0) {
        printf("Invalid input\n");
        return 1;
    }

    // fork loop
    for (int i = 0; i < nprocs; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("Child PID %d working (not implemented yet)\n", getpid());
            exit(0);
        }
    }

    for (int i = 0; i < nprocs; i++) {
        wait(NULL);
    }

    printf("Parent: All children finished (but no primes yet)\n");
    return 0;
}
