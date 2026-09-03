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

#endif
