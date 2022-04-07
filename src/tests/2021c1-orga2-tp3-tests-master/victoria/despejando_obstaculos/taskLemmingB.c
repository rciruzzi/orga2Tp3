#include "syscall.h"

void task(void)
{
  while (1)
  {
    switch (syscall_move(LEFT))
    {
    case WALL:
      syscall_explode();
      break;
    case LAKE:
      syscall_bridge(LEFT);
      break;
    default:
      break;
    }
  }
}
