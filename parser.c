#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
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

    if (end == line || *end != '\0' || value <= 0 || value > INT_MAX) {
        fprintf(stderr, "erro: tempo total de simulacao invalido\n");
        return -1;
    }

    *total_time = (int) value;
    return 0;
}

static int parse_task_line(const char *line, int order, Task *task) {
    char name[MAX_NAME_LEN];
    char period_str[32], deadline_str[32], burst_str[32];

    int matched = sscanf(line, "%31s %31s %31s %31s", name, period_str, deadline_str, burst_str);
    if (matched != 4) {
        fprintf(stderr, "erro: linha de tarefa malformada: %s\n", line);
        return -1;
    }

    char *end;

    long period = strtol(period_str, &end, 10);
    if (end == period_str || *end != '\0' || period <= 0 || period > INT_MAX) {
        fprintf(stderr, "erro: periodo invalido na tarefa %s: %s\n", name, period_str);
        return -1;
    }

    long deadline = strtol(deadline_str, &end, 10);
    if (end == deadline_str || *end != '\0' || deadline <= 0 || deadline > INT_MAX) {
        fprintf(stderr, "erro: deadline invalido na tarefa %s: %s\n", name, deadline_str);
        return -1;
    }

    long burst = strtol(burst_str, &end, 10);
    if (end == burst_str || *end != '\0' || burst <= 0 || burst > INT_MAX) {
        fprintf(stderr, "erro: burst invalido na tarefa %s: %s\n", name, burst_str);
        return -1;
    }

    if (deadline > period || burst > deadline) {
        fprintf(stderr, "erro: tarefa %s viola C <= D <= P\n", name);
        return -1;
    }

    strncpy(task->name, name, MAX_NAME_LEN - 1);
    task->name[MAX_NAME_LEN - 1] = '\0';
    task->period = (int) period;
    task->deadline = (int) deadline;
    task->burst = (int) burst;
    task->order = order;

    return 0;
}

static FILE *open_input_file(const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp)
        fprintf(stderr, "erro: nao foi possivel abrir o arquivo %s\n", path);
    return fp;
}

int load_tasks(const char *path, int *total_time, Task **tasks, int *num_tasks) {
    FILE *fp = open_input_file(path);
    if (!fp)
        return -1;

    if (read_total_time(fp, total_time) != 0) {
        fclose(fp);
        return -1;
    }

    Task *list = NULL;
    int count = 0;
    int capacity = 0;
    char line[256];

    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0')
            continue;

        if (count == capacity) {
            capacity = capacity == 0 ? 8 : capacity * 2;
            Task *grown = realloc(list, capacity * sizeof(Task));
            if (!grown) {
                fprintf(stderr, "erro: sem memoria para carregar as tarefas\n");
                free(list);
                fclose(fp);
                return -1;
            }
            list = grown;
        }

        if (parse_task_line(line, count, &list[count]) != 0) {
            free(list);
            fclose(fp);
            return -1;
        }
        count++;
    }

    fclose(fp);

    if (count == 0) {
        fprintf(stderr, "erro: arquivo nao contem nenhuma tarefa\n");
        free(list);
        return -1;
    }

    *tasks = list;
    *num_tasks = count;
    return 0;
}
