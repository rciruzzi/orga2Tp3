#include "syscall.h"
#include "types.h"

static direction_e opposite_dir(direction_e dir);

// Chequear que el resultado de una operacion sea esperado.
// En caso de que falle, se generara un breakpoint.
#define CHECK_EQ(A, B)                 \
  do {                                 \
    if ((A) != (B)) {                  \
      __asm volatile("xchg %bx, %bx"); \
    }                                  \
  }                                    \
  while (0)

// Hacer que el Lemming produzca una excepcion.
#define SEPPUKKU(...) do { __builtin_trap(); } while(0)

void task(void) {
  // Walk up to the start of the labirinth
  CHECK_EQ(syscall_move(UP), SUCCESS);
  CHECK_EQ(syscall_move(UP), SUCCESS);
  CHECK_EQ(syscall_move(UP), SUCCESS);
  CHECK_EQ(syscall_move(RIGHT), SUCCESS);
  CHECK_EQ(syscall_move(RIGHT), SUCCESS);
  CHECK_EQ(syscall_move(RIGHT), SUCCESS);
  CHECK_EQ(syscall_move(RIGHT), SUCCESS);
  CHECK_EQ(syscall_move(RIGHT), SUCCESS);
  CHECK_EQ(syscall_move(RIGHT), SUCCESS);
  CHECK_EQ(syscall_move(RIGHT), SUCCESS);
  CHECK_EQ(syscall_move(RIGHT), SUCCESS);
  
  
  direction_e dirs[] = { UP, DOWN, RIGHT, LEFT };
  direction_e last_dir = DOWN; // last successfull direction.
  size_t i = 1;
  while (true) {
    direction_e next_dir;

    do {
      i = (i + 1) % 4;
      next_dir = dirs[i];
    } while (next_dir == opposite_dir(last_dir)); // never look back.

    while (syscall_move(next_dir) == SUCCESS) {
      last_dir = next_dir;
    };

    // Si llegamos hasta aca y nos pudimos mover ok
    // last_dir == next_dir.
    // Si fallo el primer movimiento, last_dir esta igual que antes.
  }
}

static direction_e opposite_dir(direction_e dir) {
  switch (dir) {
    case UP:
      return DOWN;
    case DOWN:
      return UP;
    case LEFT:
      return RIGHT;
    case RIGHT:
      return LEFT;
    default:
      __asm volatile("xchg %bx, %bx");    
  }
  SEPPUKKU();
}