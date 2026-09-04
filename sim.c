#include "scheduler.h"

static int higher_priority_rate(const Task *tasks, int a, int b) {
    if (tasks[a].period != tasks[b].period)
        return tasks[a].period < tasks[b].period;
    return tasks[a].order < tasks[b].order;
}

static int pick_next_task_rate(const Task *tasks, const TaskInstance *instances, int num_tasks) {
    int best = -1;

    for (int i = 0; i < num_tasks; i++) {
        if (!instances[i].active || instances[i].remaining <= 0)
            continue;

        if (best == -1 || higher_priority_rate(tasks, i, best))
            best = i;
    }

    return best;
}
