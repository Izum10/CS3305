#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

int summation(int start, int end)
{
    int sum = 0;
    if (start < end)
    {
        sum = ((end * (end + 1)) - (start * (start - 1))) / 2;
    }
    return sum;
}

int ith_part_start(int i, int N, int M)
{
    int part_size = N / M;
    int start = i * part_size;
    return start;
}
int ith_part_end(int i, int N, int M)
{
    int part_size = N / M;
    int end = (i < M - 1) ? ((i + 1) * part_size - 1) : N;
    return end;
}

int main(int argc, char **argv) {
    // Check if the correct number of command-line arguments are provided.
    if (argc != 3) {
        printf("Usage: %s <N> <M>\n", argv[0]);
        return 1;
    }

    // Convert command-line arguments to integers for N and M.
    int N = atoi(argv[1]);
    int M = atoi(argv[2]);

    // Check if the values of N and M fall within the specified constraints.
    if (N < 1 || N > 100 || M < 1 || M > 10) {
        printf("Error: N and M must satisfy 1 <= N <= 100 and 1 <= M <= 10\n");
        return 1;
    }

    // Print the starting message for the parent process.
    printf("parent(PID %d): process started.\n\n", getpid());
    printf("parent(PID %d): forking child_1.\n", getpid());

    // Fork the process to create a child process (child_1).
    pid_t child1_pid = fork();
    if (child1_pid < 0) {
        // Forking error handling.
        printf("Forking error\n");
        return 1;
    } else if (child1_pid == 0) {
        // Code executed by child_1.
        printf("parent(PID %d): fork successful for child_1(PID %d)\n", getppid(), getpid());
        printf("parent(PID %d): waiting for child_1(PID %d) to complete\n\n", getppid(), getpid());
        printf("child_1(PID %d): process started from parent(PID %d)\n", getpid(), getppid());
        printf("child_1(PID %d): forking child_1.1.....child_1.%d\n\n", getpid(), M);

        int pipe_fd[2];
        int total_sum = 0;

        // Create a pipe for communication between child_1 and its child processes.
        if (pipe(pipe_fd) == -1) {
            // Pipe creation error handling.
            printf("Pipe creation error\n");
            return 1;
        }

        pid_t current_child_pid;

        // Create child processes (child_1.x) to perform partial summations.
        for (int i = 0; i < M; i++) {
            current_child_pid = fork();

            if (current_child_pid < 0) {
                // Forking error handling for child_1.x.
                printf("Forking child_1.%d error\n", (i + 1));
                return 1;
            } else if (current_child_pid == 0) {
                // Code executed by child_1.x.
                int partial_sum;
                close(pipe_fd[0]);
                printf("child_1.%d(PID %d): fork() successful\n", (i + 1), getpid());

                // Calculate the partial sum for child_1.x.
                partial_sum = summation(ith_part_start(i, N, M), ith_part_end(i, N, M));
                printf("child_1.%d(PID %d): partial sum: [%d - %d] = %d \n", (i + 1), getpid(), ith_part_start(i, N, M), ith_part_end(i, N, M), partial_sum);

                // Write the partial sum to the pipe for communication with child_1.
                write(pipe_fd[1], &partial_sum, sizeof(int));
                close(pipe_fd[1]);

                return 0;
            }
        }

        close(pipe_fd[1]);
        int child_sum;

        // Wait for child processes to complete and accumulate their partial sums.
        for (int i = 0; i < M; i++) {
            wait(NULL);
            read(pipe_fd[0], &child_sum, sizeof(int));
            total_sum += child_sum;
        }
        close(pipe_fd[0]);

        // Print the total sum calculated by child_1.
        printf("\nchild_1(PID %d): total sum = %d \n", getpid(), total_sum);
    } else if (child1_pid > 0) {
        // Code executed by the parent.
        // Wait for child_1 to complete its execution.
        wait(NULL);
        printf("child_1(PID %d): child_1 completed\n", getpid());
        printf("\nparent(PID %d): parent completed\n", getpid());
    }

    return 0;
}