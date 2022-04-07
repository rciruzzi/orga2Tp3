#include "syscall.h"

// Chequear que el resultado de una operacion sea esperado.
// En caso de que falle, se generara un breakpoint.
#define CHECK_EQ(A, B)                                                         \
  do {                                                                         \
    if ((A) != (B)) {                                                          \
      __asm volatile("xchg %bx, %bx");                                         \
    }                                                                          \
  } while (0)

// Hacer que el Lemming produzca una excepcion.
#define SEPPUKKU(...)                                                          \
  do {                                                                         \
    __asm volatile("ud2");                                                     \
  } while (0)

void task(void) {
  CHECK_EQ(syscall_move(UP), SUCCESS);
  CHECK_EQ(syscall_move(UP), SUCCESS);
  if (syscall_move(UP) == LAKE) {
    syscall_bridge(UP);
  }
  CHECK_EQ(syscall_move(UP), SUCCESS);
  CHECK_EQ(syscall_move(RIGHT), SUCCESS);
  CHECK_EQ(syscall_move(RIGHT), SUCCESS);
  CHECK_EQ(syscall_move(RIGHT), SUCCESS);
  if (syscall_move(RIGHT) == LAKE) {
    syscall_bridge(RIGHT);
  }
  for (int i = 0; i < 13; i++) {
    CHECK_EQ(syscall_move(RIGHT), SUCCESS);
  }
  if (syscall_move(DOWN) == LAKE) {
    syscall_bridge(DOWN);
  }

  while (true) {
    if (syscall_move(LEFT) != SUCCESS) {
      syscall_bridge(LEFT);
    }
  }
}
