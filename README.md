# Implementação 3 - Escalonamento (rate-monotonic vs EDF)

Simulador de escalonamento de tarefas periódicas de tempo real, comparando
rate-monotonic (rate) e earliest-deadline-first (edf).

## Arquivos

- **scheduler.h** - structs (Task, TaskInstance, Segment, TaskStats, Algorithm) e protótipos.
- **parser.c** - lê e valida o arquivo de entrada (`load_tasks`).
- **sim.c** - motor da simulação, tick a tick (`run_simulation`).
- **output.c** - escreve o `.out` no formato do enunciado (`write_output`).
- **main.c** - lê os argumentos e chama parser -> sim -> output.

## Compilar

```
make
```

Gera o executável `scheduler`. Para limpar:

```
make clean
```

## Executar

```
./scheduler rate arquivo.txt
./scheduler edf arquivo.txt
```

Gera `rate_lagi.out` ou `edf_lagi.out`. Nada é impresso em stdout na execução
normal. Entrada inválida: erro em stderr, exit code != 0, nenhum `.out` criado.

## Testar

Todos os testes (exemplo do enunciado, empate de prioridade, contador KILLED,
casos de erro) estão registrados com comando e saída em `evidencias.log`.

## Sistema operacional

Desenvolvido no Windows 11, compilado e testado via WSL (Ubuntu), gcc 15.2.0,
GNU Make 4.4.1.
