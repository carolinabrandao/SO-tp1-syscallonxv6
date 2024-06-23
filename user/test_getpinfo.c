#include "user.h"

int main() {
    struct pstat pinfo;
    if (getpinfo(&pinfo) < 0) {
        printf("Error: getpinfo failed\n");
        exit(1);
    }

    printf("PID     Tickets     Ticks\n");
    printf("-------------------------\n");
    for (int i = 0; i < NPROC; i++) {
        if (pinfo.inuse[i]) {
            printf("%d       %d           %d\n", pinfo.pid[i], pinfo.tickets[i], pinfo.ticks[i]);
        }
    }

    exit(0);
}