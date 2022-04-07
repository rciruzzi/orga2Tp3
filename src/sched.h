/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de funciones del scheduler.
*/

#ifndef __SCHED_H__
#define __SCHED_H__

#include "types.h"
#include "mapa.h"
#include "game.h"
#include "tss.h"
#include "mmu.h"
#include "gdt.h"
#include "i386.h"
#include "debug.h"

void sched_init();
uint16_t sched_next_task();
uint32_t trAnroTarea(void);
void chequearCiclos(void);
void actualizarPanelSpawneados(int equipo);

int nroTareaActual[2];
int equipoActual;
int nroTarea_a_borrar_tss;

#endif //  __SCHED_H__
