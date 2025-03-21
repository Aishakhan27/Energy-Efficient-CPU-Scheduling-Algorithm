#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For sleep() function

#define MAX_TASKS 5
#define HIGH_FREQUENCY 2.0   // GHz
#define LOW_FREQUENCY 1.0    // GHz
#define IDLE_POWER 0.3       // Watts during idle
#define ACTIVE_POWER 1.2     // Watts at high frequency

typedef struct {
    int id;
    int execution_time; // in ms
    int priority;       // Lower value = Higher priority
} Task;

// Dynamic Frequency Scaling function
double adjust_frequency(int load) {
    return (load > 70) ? HIGH_FREQUENCY : LOW_FREQUENCY;
}

// Power Consumption Calculation
double calculate_power(double frequency, int is_idle) {
    return is_idle ? IDLE_POWER : ACTIVE_POWER * (frequency / HIGH_FREQUENCY);
}

// Power-Aware Priority Sorting
void sort_tasks_by_priority(Task tasks[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (tasks[j].priority > tasks[j + 1].priority) {
                Task temp = tasks[j];
                tasks[j] = tasks[j + 1];
                tasks[j + 1] = temp;
            }
        }
    }
}

// CPU Scheduling Simulation
void schedule_tasks(Task tasks[], int n) {
    int total_load = 0;
    for (int i = 0; i < n; i++) total_load += tasks[i].execution_time;

    sort_tasks_by_priority(tasks, n);

    printf("Scheduling Tasks...\n");
    for (int i = 0; i < n; i++) {
        double freq = adjust_frequency((tasks[i].execution_time * 100) / total_load);
        double power = calculate_power(freq, 0);

        printf("Task %d | Priority: %d | Exec Time: %d ms | Frequency: %.1f GHz | Power: %.2f W\n",
               tasks[i].id, tasks[i].priority, tasks[i].execution_time, freq, power);

        usleep(tasks[i].execution_time * 1000); // Simulate task execution
    }

    printf("System entering idle state...\n");
    printf("Idle Power Consumption: %.2f W\n", calculate_power(LOW_FREQUENCY, 1));
}

int main() {
    Task tasks[MAX_TASKS] = {
        {1, 200, 2},
        {2, 100, 1},
        {3, 300, 3},
        {4, 150, 2},
        {5, 250, 1}
    };

    schedule_tasks(tasks, MAX_TASKS);

    return 0;
}

