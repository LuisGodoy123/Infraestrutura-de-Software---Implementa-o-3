#ifndef SCHEDULER_H
#define SCHEDULER_H

#define MAX_NAME_LEN 32

typedef struct {
    char name[MAX_NAME_LEN];
    int period;
    int deadline;
    int burst;
    int order;
} Task;

typedef enum {
    ALG_RATE,
    ALG_EDF
} Algorithm;

typedef struct {
    int active;
    int remaining;
    int arrival;
    int absolute_deadline;
} TaskInstance;

typedef struct {
    int completed;
    int lost;
    int killed;
} TaskStats;

int load_tasks(const char *path, int *total_time, Task **tasks, int *num_tasks);

#endif
