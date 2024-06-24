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
        settickets(tickets1);
        while (1) {
            spin();
        }
    }

    int pid2 = fork();
    if (pid2 == 0) {
        settickets(tickets2);
        while (1) {
            spin();
        }
    }

    int pid3 = fork();
    if (pid3 == 0) {
        settickets(tickets3);
        while (1) {
            spin();
        }
    }

    // Let the processes run for a while
    sleep(500);

    struct pstat pinfo;
    if (getpinfo(&pinfo) < 0) {
        printf("Error: getpinfo failed\n");
        exit(1);
    }

    printf("PID\tTickets\tTicks\n");
    printf("-------------------------\n");
    for (int i = 0; i < NPROC; i++) {
        if (pinfo.inuse[i]) {
            printf("%d\t%d\t%d\n", pinfo.pid[i], pinfo.tickets[i], pinfo.ticks[i]);
        }
    }

    // Kill child processes
    // Kill child processes
    printf("Killing child process with PID %d\n", pid1);
    kill(pid1);
    printf("Killed child process with PID %d\n", pid1);
    printf("Killing child process with PID %d\n", pid2);
    kill(pid2);
    printf("Killed child process with PID %d\n", pid2);
    printf("Killing child process with PID %d\n", pid3);
    kill(pid3);
    printf("Killed child process with PID %d\n", pid3);

    // Wait for children to exit
    printf("Waiting for child process with PID %d to exit\n", pid1);
    wait(0);
    printf("Waiting for child process with PID %d to exit\n", pid2);
    wait(0);
    printf("Waiting for child process with PID %d to exit\n", pid3);
    wait(0);

    printf("TEST PASSED\n");    

    exit(0);
}
