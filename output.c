#include <stdio.h>
#include "scheduler.h"

static void write_segment(FILE *fp, const Task *tasks, const Segment *seg) {
    if (seg->task_index == -1) {
        fprintf(fp, "idle for %d units\n", seg->duration);
        return;
    }

    if (seg->outcome)
        fprintf(fp, "[%s] for %d units - %c\n", tasks[seg->task_index].name, seg->duration, seg->outcome);
    else
        fprintf(fp, "[%s] for %d units\n", tasks[seg->task_index].name, seg->duration);
}

int write_output(const char *login, Algorithm alg, const Task *tasks, int num_tasks,
                  const Segment *segments, int num_segments, const TaskStats *stats) {
    char filename[64];
    snprintf(filename, sizeof(filename), "%s_%s.out", alg == ALG_RATE ? "rate" : "edf", login);

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "erro: nao foi possivel criar o arquivo de saida %s\n", filename);
        return -1;
    }

    fprintf(fp, "EXECUTION BY %s\n\n", alg == ALG_RATE ? "RATE" : "EDF");
    for (int i = 0; i < num_segments; i++)
        write_segment(fp, tasks, &segments[i]);

    fprintf(fp, "\nLOST DEADLINES\n");
    for (int i = 0; i < num_tasks; i++)
        fprintf(fp, "[%s] %d\n", tasks[i].name, stats[i].lost);

    fprintf(fp, "\nCOMPLETE EXECUTION\n");
    for (int i = 0; i < num_tasks; i++)
        fprintf(fp, "[%s] %d\n", tasks[i].name, stats[i].completed);

    fprintf(fp, "\nKILLED\n");
    for (int i = 0; i < num_tasks; i++)
        fprintf(fp, "[%s] %d\n", tasks[i].name, stats[i].killed);

    fclose(fp);
    return 0;
}
