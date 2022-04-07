#include "syscall.h"

void task(void)
{
  while (1)
  {
    switch (syscall_move(RIGHT))
    {
    case WALL:
      syscall_explode();
      break;
    case LAKE:
      syscall_bridge(RIGHT);
      break;
    default:
      break;
    }
  }
}
