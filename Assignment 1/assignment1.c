#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// Main Function
int main(int argc, char *argv[])
{
    char *n = argv[1], *m = argv[2], *s = argv[3], *prg1= argv[4], *prg2=argv[5];
    
    printf("parent(PID %d): process started\n", getpid());
    printf("parent(PID %d): forking child_1\n\n", getpid());

    pid_t pid = fork();
    
    // Creating Child 1
    if (pid == 0)
    {
        printf("parent (PID %d): fork successful for child_1 (PID %d)\n", getppid(), getpid());
        printf("parent (PID %d): waiting for child_1 (PID %d) to complete\n", getppid(), getpid());
        printf("child_1 (PID %d): process started from parent (PID %d)\n\n", getpid(), getppid());

        printf("child_1 (PID %d): forking child_1.1\n", getpid());
        pid_t pid_1 = fork();

        if (pid_1 < 0)
        {
            printf("Error. Fork Failed in Making Child 1.1\n");
            exit(1);
        }
        else if (pid_1 == 0)
        {
            // Child 1.1
            printf("child_1.1 (PID %d): process started from child_1 (PID %d)\n", getpid(), getppid());
            printf("child_1.1 (PID %d): calling an external program [./external_program1.out]\n", getpid());
            execl(prg1, n, NULL);
        }
        else if (pid_1 > 0)
        {
            // Child 1.2
            wait(NULL);
            printf("child_1 (PID %d): completed child_1.1\n\n", getpid());
            printf("child_1 (PID %d): forking child_1.2\n", getpid());
            pid_t pid1_2 = fork();
            if (pid1_2 < 0)
            {
                printf("Error. Fork Failed in Making Child 1.2\n");
                exit(1);
            }
            else if (pid1_2 == 0)
            {
                printf("child_1.2 (PID %d): process started from child_1 (PID %d)\n", getpid(), getppid());
                printf("child_1.2 (PID %d): calling an external program [./external_program1.out]\n", getpid());
                execl(prg1, m, NULL);
            }
            else if (pid1_2 > 0)
            {
                wait(NULL);
                printf("child_1 (PID %d): completed child_1.2\n\n", getpid());
            }
        }
    }
    
    // Creating Child 2
    if (pid > 0)
    {
        wait(NULL);

        printf("parent (PID %d): forking child_2\n", getpid());
        pid_t pid2 = fork();

        if (pid2 < 0)
        {
            printf("Error. Fork Failed in Making Child 2");
            exit(1);
        }
        else if (pid2 == 0)
        {
            printf("parent (PID %d): fork successful for child_2 (PID %d)\n", getppid(), getpid());
            printf("child_2 (PID %d): process started from parent (PID %d)\n", getpid(), getppid());
            execl(prg2, s, NULL);
        }
        else if (pid2 > 0)
        {
            wait(NULL);
            printf("parent (PID %d): completed parent\n", getpid());
        }
    }

    // Check Error to see if Fork Failed
    if (pid < 0)
    {
        printf("Error. Fork has Failed");
        exit(1);
    }
}