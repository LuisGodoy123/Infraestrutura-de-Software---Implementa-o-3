#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduler.h"

#define LOGIN "lagi"

static int parse_algorithm(const char *arg, Algorithm *alg) {
    if (strcmp(arg, "rate") == 0) {
        *alg = ALG_RATE;
        return 0;
    }
    if (strcmp(arg, "edf") == 0) {
        *alg = ALG_EDF;
        return 0;
    }

    fprintf(stderr, "erro: algoritmo invalido '%s', use 'rate' ou 'edf'\n", arg);
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "uso: %s <rate|edf> <arquivo_de_entrada>\n", argv[0]);
        return 1;
    }

    Algorithm alg;
    if (parse_algorithm(argv[1], &alg) != 0)
        return 1;

    int total_time;
    Task *tasks;
    int num_tasks;
    if (load_tasks(argv[2], &total_time, &tasks, &num_tasks) != 0)
        return 1;

    Segment *segments;
    int num_segments;
    TaskStats *stats;
    run_simulation(alg, tasks, num_tasks, total_time, &segments, &num_segments, &stats);

    int result = write_output(LOGIN, alg, tasks, num_tasks, segments, num_segments, stats);

    free(tasks);
    free(segments);
    free(stats);

    return result == 0 ? 0 : 1;
}
