#include "user.h"
#include "stddef.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: getcnt syscall_num\n");
    exit(1);
  }
  int syscall_num = atoi(argv[1]);
  int count = getcnt(syscall_num);
  printf("syscall %d has been called %d times\n", syscall_num, count);
  exit(0);
}
