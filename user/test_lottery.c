#include "user.h"


int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: test_lottery tickets1 tickets2 tickets3\n");
        exit(1);
    }

    // Conversão dos argumentos para inteiros
    int tickets_a = atoi(argv[1]);
    int tickets_b = atoi(argv[2]);
    int tickets_c = atoi(argv[3]);

    // Criação do primeiro processo filho
    int process_a = fork();
    if (process_a == 0) {
        settickets(tickets_a);
        while (1) {
            int counter = 0;
            while (counter < 10000000) {
                counter++;
            }
        }
    }

    // Criação do segundo processo filho
    int process_b = fork();
    if (process_b == 0) {
        settickets(tickets_b);
        while (1) {
            int counter = 0;
            while (counter < 10000000) {
                counter++;
            }
        }
    }

    // Criação do terceiro processo filho
    int process_c = fork();
    if (process_c == 0) {
        settickets(tickets_c);
        while (1) {
            int counter = 0;
            while (counter < 10000000) {
                counter++;
            }
        }
    }

    struct pstat proc_info;
    printf("step,pid,tickets,ticks\n");

    // Coleta e impressão de informações dos processos em execução
    for (int step = 0; step < 100; step++) {
        sleep(10);

        if (getpinfo(&proc_info) < 0) {
            printf("Error: getpinfo failed\n");
            exit(1);
        }

        // Impressão de dados dos processos filhos específicos
        for (int i = 0; i < NPROC; i++) {
            if (proc_info.inuse[i] && proc_info.tickets[i] > 1) {
                printf("%d,%d,%d,%d\n", step, proc_info.pid[i], proc_info.tickets[i], proc_info.ticks[i]);
            }
        }
    }

    // Encerramento dos processos filhos
    kill(process_a);
    kill(process_b);
    kill(process_c);

    // Espera pelos processos filhos para terminar
    wait(0);
    wait(0);
    wait(0);

    exit(0);
}