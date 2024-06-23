#include "user.h"

int main(int argc, char *argv[]) {
   struct pstat pinfo;
    if (getpinfo(&pinfo) < 0) {
        printf("Error: getpinfo failed\n");
        exit(1);
    }

    for (int i = 0; i < NPROC; i++) {
        if (pinfo.inuse[i]) {
            printf("PID: %d, Tickets: %d, Ticks: %d\n", pinfo.pid[i], pinfo.tickets[i], pinfo.ticks[i]);
        }
    }
    exit(0);
}
