#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Structure to represent a process in the system
struct Process {
    int number;            // Process ID
    int arrival_time;      // Time at which the process arrives
    int burst_time;        // CPU burst time for the process
    int waiting_time;      // Waiting time for the process
    int turnaround_time;   // Turnaround time for the process
    int completed;         // Flag to check if the process is completed
    int start_time;        // Time at which the process starts execution
};

// Comparator function for qsort() to sort processes by arrival time and burst time
int compare(const void *a, const void *b) {
    struct Process *p1 = (struct Process *)a;
    struct Process *p2 = (struct Process *)b;
    if (p1->arrival_time == p2->arrival_time) {
        return p1->burst_time - p2->burst_time;
    }
    return p1->arrival_time - p2->arrival_time;
}

// Function to calculate waiting and turnaround times for each process
void calculate_times(struct Process processes[], int n) {
    int current_time = 0;               // Tracks the current time
    int completed_processes = 0;        // Counts how many processes have been completed
    float total_turnaround_time = 0;    // Sums up the turnaround time of all processes
    float total_waiting_time = 0;       // Sums up the waiting time of all processes
    int is_processing = 0;              // Indicates if a process is currently being executed

    // Loop until all processes are completed
    while (completed_processes < n) {
        if (!is_processing) {
            int shortest_burst_time = INT_MAX;
            struct Process *next_process = NULL;

            // Find the next process to execute based on SJF (Shortest Job First)
            for (int i = 0; i < n; i++) {
                if (!processes[i].completed && processes[i].arrival_time <= current_time && processes[i].burst_time < shortest_burst_time) {
                    shortest_burst_time = processes[i].burst_time;
                    next_process = &processes[i];
                }
            }

            // If there is a process ready to be processed
            if (next_process != NULL) {
                is_processing = 1;
                next_process->start_time = current_time;  // Set the start time for the process

                // Execute the process burst time
                for (int j = 0; j <= next_process->burst_time; j++) {
                    printf("CPU Time %d: ", current_time);

                    // Check if any process arrives at the current time
                    for (int k = 0; k < n; k++) {
                        if (processes[k].arrival_time == current_time ) {
                            printf("[P%d Arrived] ", processes[k].number);
                        }
                    }
                    // Print the progress of the current process
                    if (j == next_process->burst_time) {
                        printf("P%d [%d/%d]\n", next_process->number, j, next_process->burst_time);
                        current_time--;  // Adjust for the loop's next iteration increment
                    } else {
                        printf("P%d [%d/%d]\n", next_process->number, j, next_process->burst_time);
                    }
                    current_time++;
                }

                // Update process completion and timing information
                next_process->completed = 1;
                next_process->turnaround_time = current_time - next_process->arrival_time;
                next_process->waiting_time = next_process->start_time - next_process->arrival_time;

                // Print the completion information for the process
                printf("Process P%d completed with Turnaround Time: %d, Waiting Time: %d\n",
                       next_process->number,
                       next_process->turnaround_time,
                       next_process->waiting_time);

                // Update the total turnaround and waiting times
                total_turnaround_time += next_process->turnaround_time;
                total_waiting_time += next_process->waiting_time;
                completed_processes++;
                is_processing = 0;
            } else {
                // No process is ready to execute at the current time
                printf("CPU Time %d: None\n", current_time);
                current_time++;
            }
        }
    }

    // Print the average turnaround and waiting times after all processes are scheduled
    printf("Process scheduling completed with Avg Turnaround Time: %.2f, Avg Waiting Time: %.2f\n",
           total_turnaround_time / n,
           total_waiting_time / n);
}

int main() {
    struct Process processes[100];  // Array to hold all processes
    int process_count = 0;          // Counter for the number of processes
    FILE *file = fopen("sjf_input.txt", "r"); // Open the file for reading
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    char buffer[1024];  // Buffer to hold file lines
    int testCase = 0;   // Counter for the number of test cases

    // Read each line from the file
    while (fgets(buffer, sizeof(buffer), file)) {
        char *line = strtok(buffer, "\n");
        process_count = 0;

        // Parse each process from the line
        while (*line != '\0') {
            if (*line == 'p') {
                line++;
                processes[process_count].number = strtol(line, &line, 10);
                line++;
                processes[process_count].arrival_time = strtol(line, &line, 10);
                line++;
                processes[process_count].burst_time = strtol(line, &line, 10);
                processes[process_count].completed = 0;
                processes[process_count].start_time = -1;
                process_count++;
            } else {
                line++;
            }
        }

        // Sort the processes by arrival time and burst time
        qsort(processes, process_count, sizeof(struct Process), compare);

        // Print out the test case information
        testCase++;
        printf("Test Case #%d: ", testCase);
        for (int j = 0; j < process_count; j++) {
            printf("p%d %d %d ", processes[j].number, processes[j].arrival_time, processes[j].burst_time);
        }
        printf("\nNumber of Processes: %d\n", process_count);
        printf("Process Scheduling Started:\n");
        calculate_times(processes, process_count);
        printf("\n");

        // Clear the buffer for the next read
        memset(buffer, 0, sizeof(buffer));
    }

    fclose(file); // Close the file
    return 0;
}