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
        if (line[0] == '\n' || line[0] == '\0')
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
