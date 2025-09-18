#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>

int is_prime(int n) {
    if (n < 2) return 0;
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) return 0;0
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
    if (nprocs > total) {
        nprocs = total;
    }

    int base = total / nprocs;
    int rem = total % nprocs;

    int start = lower;
    for (int i = 0; i < nprocs; i++) {
        int size = base + (i < rem ? 1 : 0);
        int end = start + size - 1;

        pid_t pid = fork();
        if (pid == 0) {
            printf("Child PID %d checking range [%d, %d]\n", getpid(), start, end);
            // TODO: check for primes later
            exit(0);
        } else if (pid < 0) {
            perror("fork");
            exit(1);
        }

        start = end + 1;
    }

    for (int i = 0; i < nprocs; i++) {
        wait(NULL);
    }

    printf("Parent: All children finished (no primes yet)\n");
    return 0;
}
