#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

int input_array[4];

// This function is executed by thread_1.
// It reads X and Y from input_array[], calculates their sum, and writes the result to input_array[2].
void *sum(void *thread_id) {
    int X = input_array[0];
    int Y = input_array[1];
    int S = X + Y;
    input_array[2] = S;
    printf("thread_1 (TID %lu) reads X = %d and Y = %d from input_array[]\n", (unsigned long)pthread_self(), X, Y);
    printf("thread_1 (TID %lu) writes X + Y = %d to input_array[2]\n", (unsigned long)pthread_self(), S);
    return NULL;
}

// This function is executed by thread_2.
// It reads S from input_array[2] and identifies if it's an even or odd number.
void *even_odd(void *thread_id) {
    int S = input_array[2];
    printf("thread_2 (TID %lu) reads %d from input_array[2]\n", (unsigned long)pthread_self(), S);
    if (S % 2 == 0) {
        printf("thread_2 (TID %lu) identified that %d is even\n", (unsigned long)pthread_self(), S);
    } else {
        printf("thread_2 (TID %lu) identifies that %d is odd\n", (unsigned long)pthread_self(), S);
    }
    return NULL;
}

// This function is executed by thread_2.
// It reads X and Y from input_array[], calculates their product, and writes the result to input_array[3].
void *multiplication(void *thread_id) {
    int X = input_array[0];
    int Y = input_array[1];
    int M = X * Y;
    input_array[3] = M;
    printf("thread_2 (TID %lu) reads X and Y from input_array[], writes X * Y = %d to input_array[3]\n", (unsigned long)pthread_self(), M);
    return NULL;
}

// This function is executed by thread_3.
// It reads M from input_array[3], reverses it, and prints the result.
void *reverse_num(void *thread_id) {
    int M = input_array[3];
    char str[100];
    sprintf(str, "%d", M);
    int len = strlen(str);
    char reversed[len+1];
    for (int i = 0; i < len; i++) {
        reversed[i] = str[len - i - 1];
    }
    reversed[len] = '\0';
    printf("thread_3 (TID %lu) reads %d from input_array[3]\n", (unsigned long)pthread_self(), M);
    printf("thread_3 (TID %lu) reverses the number %d -> %s\n", (unsigned long)pthread_self(), M, reversed);
    return NULL;
}

int main(int argc, char *argv[]) {
    // Check for the correct number of command-line arguments.
    if (argc != 3) {
        printf("Usage: %s <X> <Y>\n", argv[0]);
        return 1;
    }

    // Assign command-line arguments to input_array.
    input_array[0] = atoi(argv[1]);
    input_array[1] = atoi(argv[2]);
    printf("parent (PID %d) receives X = %d and Y = %d from the user\n", getpid(), input_array[0], input_array[1]);
    printf("parent (PID %d) writes X = %d and Y = %d to input_array[]\n", getpid(), input_array[0], input_array[1]);
    
    // Thread variables
    pthread_t thread_1, thread_2a, thread_2b, thread_3;

    // Create and wait for thread_1 to complete.
    pthread_create(&thread_1, NULL, sum, NULL);
    pthread_join(thread_1, NULL);

    // Create and wait for thread_2a (even/odd checker) to complete.
    pthread_create(&thread_2a, NULL, even_odd, NULL);
    pthread_join(thread_2a, NULL); 

    // Create and wait for thread_2b (multiplication) to complete.
    pthread_create(&thread_2b, NULL, multiplication, NULL);
    pthread_join(thread_2b, NULL); 

    // Create and wait for thread_3 (number reverser) to complete.
    pthread_create(&thread_3, NULL, reverse_num, NULL);
    pthread_join(thread_3, NULL);

    return 0;
}
