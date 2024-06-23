#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "pstat.h"

extern int syscall_counts[NUM_SYSCALLS]; 

extern struct proc proc[NPROC];

extern int total_tickets;
extern struct spinlock tickets_lock;

extern struct {
    struct spinlock lock;
    struct proc proc[NPROC];
} ptable;

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


uint64
sys_getcnt(void)
{
  int syscall_number;
  argint(0, &syscall_number);
  if (syscall_number < 1 || syscall_number >= NUM_SYSCALLS){
    return -1;
  }
  return syscall_counts[syscall_number];
}

// uint64
// sys_settickets(void)
// {
//     int n;
//     argint(0, &n);
//     if(n < 1)
//       return -1;
//     myproc()->tickets = n;
//     return 0;
// }

uint64
sys_settickets(void)
{
    printf("sys_settickets\n");
    int n;
    struct proc *p = myproc(); // Obtém o processo atual
    argint(0, &n);
    if(n < 1)
      return -1;

    acquire(&p->lock);
    p->tickets = n;
    release(&p->lock);

    acquire(&tickets_lock);
    total_tickets += n;
    release(&tickets_lock);

    return 0;
}

// uint64
// sys_getpinfo(void)
// {
//     struct pstat *pstat;
//     argint(0, (int*)(&pstat));
//     if (pstat < 0)
//         return -1;

//     struct proc *p;
//     int i = 0;

//     acquire(&ptable.lock);
//     for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
//         pstat->inuse[i] = (p->state != UNUSED && p->state != ZOMBIE);
//         pstat->pid[i] = p->pid;
//         pstat->tickets[i] = p->tickets;
//         pstat->ticks[i] = p->ticks;
//         i++;
//     }
//     release(&ptable.lock);

//     return 0;
// }


uint64
sys_getpinfo(void)
{
    struct pstat pstat;
    struct pstat *upstat;

    // Obtenha o ponteiro do espaço do usuário
    argaddr(0, (uint64*)&upstat);

    if (upstat < 0)
        return -1;

    if (upstat == 0)
        return -1;

    struct proc *p;
    int i = 0;

    for (p = proc; p < &proc[NPROC]; p++) {
        acquire(&p->lock);
        if (p->state != UNUSED) {
            pstat.inuse[i] = 1;
            pstat.pid[i] = p->pid;
            pstat.tickets[i] = p->tickets;
            pstat.ticks[i] = p->ticks;
        } else {
            pstat.inuse[i] = 0;
            pstat.pid[i] = 0;
            pstat.tickets[i] = 0;
            pstat.ticks[i] = 0;
        }
        release(&p->lock);
        i++;
    }

    // Copie a estrutura preenchida para o espaço do usuário
    if (copyout(myproc()->pagetable, (uint64)upstat, (char*)&pstat, sizeof(pstat)) < 0)
        return -1;

    return 0;
}
