#include <stdlib.h>
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

static void check_deadline_misses(TaskInstance *instances, TaskStats *stats, int num_tasks, int t) {
    for (int i = 0; i < num_tasks; i++) {
        if (instances[i].active && instances[i].remaining > 0 && instances[i].absolute_deadline == t) {
            instances[i].active = 0;
            stats[i].lost++;
        }
    }
}

static void execute_one_tick(TaskInstance *instances, TaskStats *stats, int task_index) {
    if (task_index == -1)
        return;

    instances[task_index].remaining--;

    if (instances[task_index].remaining == 0) {
        instances[task_index].active = 0;
        stats[task_index].completed++;
    }
}

static void append_segment(Segment **segments, int *count, int *capacity, int task_index, int duration, char outcome) {
    if (*count == *capacity) {
        *capacity = *capacity == 0 ? 16 : *capacity * 2;
        *segments = realloc(*segments, *capacity * sizeof(Segment));
    }

    (*segments)[*count].task_index = task_index;
    (*segments)[*count].duration = duration;
    (*segments)[*count].outcome = outcome;
    (*count)++;
}

static void check_arrivals(const Task *tasks, TaskInstance *instances, int num_tasks, int t) {
    for (int i = 0; i < num_tasks; i++) {
        if (t % tasks[i].period == 0) {
            instances[i].active = 1;
            instances[i].remaining = tasks[i].burst;
            instances[i].arrival = t;
            instances[i].absolute_deadline = t + tasks[i].deadline;
        }
    }
}

static void finalize_killed_stats(const TaskInstance *instances, TaskStats *stats, int num_tasks) {
    for (int i = 0; i < num_tasks; i++) {
        if (instances[i].active && instances[i].remaining > 0)
            stats[i].killed++;
    }
}

void run_simulation(Algorithm alg, const Task *tasks, int num_tasks, int total_time,
                     Segment **out_segments, int *out_num_segments, TaskStats **out_stats) {
    TaskInstance *instances = calloc(num_tasks, sizeof(TaskInstance));
    TaskStats *stats = calloc(num_tasks, sizeof(TaskStats));

    Segment *segments = NULL;
    int seg_count = 0;
    int seg_capacity = 0;

    int running = -1;
    int segment_start = 0;

    for (int t = 0; t < total_time; t++) {
        check_deadline_misses(instances, stats, num_tasks, t);
        check_arrivals(tasks, instances, num_tasks, t);

        int chosen = (alg == ALG_RATE) ? pick_next_task_rate(tasks, instances, num_tasks) : -1;

        if (chosen != running) {
            if (t > segment_start) {
                char outcome = 0;
                if (running != -1) {
                    if (!instances[running].active)
                        outcome = (instances[running].remaining == 0) ? 'F' : 'L';
                    else
                        outcome = 'H';
                }
                append_segment(&segments, &seg_count, &seg_capacity, running, t - segment_start, outcome);
            }
            running = chosen;
            segment_start = t;
        }

        execute_one_tick(instances, stats, chosen);
    }

    char outcome = 0;
    if (running != -1 && !instances[running].active && instances[running].remaining == 0)
        outcome = 'F';
    append_segment(&segments, &seg_count, &seg_capacity, running, total_time - segment_start, outcome);

    finalize_killed_stats(instances, stats, num_tasks);
    free(instances);

    *out_segments = segments;
    *out_num_segments = seg_count;
    *out_stats = stats;
}
