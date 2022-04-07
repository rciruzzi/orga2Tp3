#include "syscall.h"

void task(void) {
  while (true) {
    __asm__ volatile("pause");
  }
}
