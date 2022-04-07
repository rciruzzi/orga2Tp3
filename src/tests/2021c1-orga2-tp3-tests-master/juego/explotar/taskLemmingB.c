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

  for (int i = 0; i < 10; i++) {
    CHECK_EQ(syscall_move(LEFT), SUCCESS);
  }

  for (int i = 0; i < 36; i++) {
    syscall_move(DOWN);
  }

  while (true) {
    __asm__ volatile("pause");
  }
}
