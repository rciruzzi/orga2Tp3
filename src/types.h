/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de tipos comunes en el kernel.
*/

#ifndef __TYPES_H__
#define __TYPES_H__

#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"


#define MMU_FLAG_PRESENT 1
#define MMU_FLAG_SUPERVISOR 0
#define MMU_FLAG_READWRITE 1
#define MMU_FLAG_READONLY 0

typedef uint32_t vaddr_t; // direccion virtual.
typedef uint32_t paddr_t; // direccion fisica.

#endif //  __TYPES_H__
