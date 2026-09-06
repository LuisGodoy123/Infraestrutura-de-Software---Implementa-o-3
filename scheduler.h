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

typedef struct {
    int task_index;
    int duration;
    char outcome;
} Segment;

int load_tasks(const char *path, int *total_time, Task **tasks, int *num_tasks);

void run_simulation(Algorithm alg, const Task *tasks, int num_tasks, int total_time,
                     Segment **segments, int *num_segments, TaskStats **stats);

int write_output(const char *login, Algorithm alg, const Task *tasks, int num_tasks,
                  const Segment *segments, int num_segments, const TaskStats *stats);

#endif
