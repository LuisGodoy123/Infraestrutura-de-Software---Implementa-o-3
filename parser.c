#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"

static int read_total_time(FILE *fp, int *total_time) {
    char line[256];

    if (!fgets(line, sizeof(line), fp)) {
        fprintf(stderr, "erro: nao foi possivel ler o tempo total de simulacao\n");
        return -1;
    }

    char *end;
    long value = strtol(line, &end, 10);

    while (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')
        end++;

    if (end == line || *end != '\0' || value <= 0) {
        fprintf(stderr, "erro: tempo total de simulacao invalido\n");
        return -1;
    }

    *total_time = (int) value;
    return 0;
}

static int parse_task_line(const char *line, int order, Task *task) {
    char name[MAX_NAME_LEN];
    int period, deadline, burst;

    int matched = sscanf(line, "%31s %d %d %d", name, &period, &deadline, &burst);
    if (matched != 4) {
        fprintf(stderr, "erro: linha de tarefa malformada: %s\n", line);
        return -1;
    }

    if (period <= 0 || deadline <= 0 || burst <= 0) {
        fprintf(stderr, "erro: valores da tarefa %s devem ser positivos\n", name);
        return -1;
    }

    if (deadline > period || burst > deadline) {
        fprintf(stderr, "erro: tarefa %s viola C <= D <= P\n", name);
        return -1;
    }

    strncpy(task->name, name, MAX_NAME_LEN - 1);
    task->name[MAX_NAME_LEN - 1] = '\0';
    task->period = period;
    task->deadline = deadline;
    task->burst = burst;
    task->order = order;

    return 0;
}
