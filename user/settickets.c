#include "user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: settickets num_tickets\n");
        exit(1);
    }

    int num_tickets = atoi(argv[1]);
    if (num_tickets < 1) {
        printf("Error: num_tickets must be greater than 0\n");
        exit(1);
    }

    if (settickets(num_tickets) < 0) {
        printf("Error: settickets failed\n");
        exit(1);
    }

    exit(0);
}