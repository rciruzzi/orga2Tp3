#include "syscall.h"

void task(void) {
  //__asm volatile("xchg %bx, %bx");
  while (1) {
    syscall_move(RIGHT);
  }
}
