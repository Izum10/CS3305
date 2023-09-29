#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
    printf("\nchild (PID %d): external_program2: Hello %s, Thanks for your effort!\n", getpid(), argv[0]);
}