/* a1p2.c
 * - parent allocates shared memory and creates n children
 * - each child gets a private subrange [start, end]
 * - each child writes its primes into its own block in shared memory
 * - parent waits for all children, reads shared memory, prints all primes
 * - parent detaches & removes shared memory
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <math.h>

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

    if (lower > upper || nprocs <= 0) {
        printf("Error: invalid input\n");
        return 1;
    }

    int total = upper - lower + 1;
    if (nprocs > total) nprocs = total;

    int base = total / nprocs;
    int rem = total % nprocs;

    // shared memory: each child gets its own block
    int max_per_child = (total + nprocs - 1) / nprocs;
    int ints_per_block = 1 + max_per_child;
    int total_ints = nprocs * ints_per_block;

    int shmid = shmget(IPC_PRIVATE, total_ints * sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        return 1;
    }

    int *shm = (int *) shmat(shmid, NULL, 0);
    if (shm == (void *) -1) {
        perror("shmat");
        shmctl(shmid, IPC_RMID, NULL);
        return 1;
    }

    // init shared memory
    for (int i = 0; i < total_ints; i++) shm[i] = 0;

    pid_t pids[nprocs];
    int start = lower;

    for (int i = 0; i < nprocs; i++) {
        int size = base + (i >= nprocs - rem ? 1 : 0); // give remainder to last children
        int end = start + size - 1;

        pid_t pid = fork();
        if (pid == 0) {
            printf("Child PID %d checking range [%d, %d]\n", getpid(), start, end);
            int *block = shm + i * ints_per_block;
            int count = 0;
            for (int v = start; v <= end; v++) {
                if (is_prime(v)) {
                    block[1 + count] = v;
                    count++;
                }
            }
            block[0] = count;
            shmdt(shm);
            exit(0);
        } else if (pid > 0) {
            pids[i] = pid;
        } else {
            perror("fork");
            return 1;
        }

        start = end + 1;
    }

    // parent waits
    for (int i = 0; i < nprocs; i++) {
        waitpid(pids[i], NULL, 0);
    }

    printf("\nParent: All children finished. Primes found:\n");
    for (int i = 0; i < nprocs; i++) {
        int *block = shm + i * ints_per_block;
        for (int k = 0; k < block[0]; k++) {
            printf("%d ", block[1 + k]);
        }
    }
    printf("\n");

    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
