#include "syscall.h"

void task(void) {
  while (1) {
    syscall_move(RIGHT);
  }
}
