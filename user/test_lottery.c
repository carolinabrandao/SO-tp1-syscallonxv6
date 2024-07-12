#include "user.h"

void spin() {
    int i = 0;
    while (i < 10000000) { // Busy-wait loop to consume CPU time
        i++;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: test_lottery tickets1 tickets2 tickets3\n");
        exit(1);
    }

    int tickets1 = atoi(argv[1]);
    int tickets2 = atoi(argv[2]);
    int tickets3 = atoi(argv[3]);

    int pid1 = fork();
    if (pid1 == 0) {
        printf("pid1\n");
        settickets(tickets1);
        while (1) {
            spin();
        }
    }

    int pid2 = fork();
    if (pid2 == 0) {
        printf("pid2\n");
        settickets(tickets2);
        while (1) {
            spin();
        }
    }

    int pid3 = fork();
    if (pid3 == 0) {
        printf("pid3\n");
        settickets(tickets3);
        while (1) {
            spin();
        }
    }


    struct pstat pinfo;
    printf("step,pid,tickets,ticks\n");
    for(int t = 0; t<100; t++){
        printf("t=%d\n", t);
        sleep(10);

        if (getpinfo(&pinfo) < 0) {
            printf("Error: getpinfo failed\n");
            exit(1);
        }
        for (int i = 0; i < NPROC; i++) {
            if (pinfo.inuse[i] && pinfo.tickets[i] > 1) {
                printf("%d,%d,%d,%d\n", t, pinfo.pid[i], pinfo.tickets[i], pinfo.ticks[i]);
            }
        }
        printf("fim\n");
    }

    // Kill child processes
    kill(pid1);
    kill(pid2);
    kill(pid3);
    
    // Wait for children to exit
    wait(0);
    wait(0);
    wait(0);

    exit(0);
}