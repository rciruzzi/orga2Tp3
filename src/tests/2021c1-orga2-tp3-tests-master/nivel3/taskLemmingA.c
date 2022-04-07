#include "syscall.h"
#include "types.h"

void task(void) {
  __asm__ volatile("mov %%cr0, %%eax" ::: "eax");

  for (int i = 0; i < 20; i++)
    syscall_move(UP);

  for (int i = 0; i < 20; i++)
    syscall_move(RIGHT);

  while (1) {
    __asm volatile("pause");
  }
}
