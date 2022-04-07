/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de estructuras para administrar tareas
*/
#include "sched.h"
#include "tss.h"
#include "defines.h"
#include "kassert.h"
#include "mmu.h"
#include "gdt.h"
#include "i386.h"

tss_t tss_initial = {0};

tss_t tss_idle = {
    //---
    //esto no hace falta ya que la tarea es de nivel 0, no va a haber cambio de privilegio
    .ss0 = GDT_IDX_DS_RING_0 << 3 | TI_0_RPL_0, 
    .esp0 = KERNEL_STACK,
    //---
    .eip = TASK_IDLE_START,
    .eflags = ENABLE_INT_FLAG,
    .eax = 0,
    .ecx = 0,
    .edx = 0,
    .ebx = 0,
    .esp = KERNEL_STACK, 
    .ebp = 0, //a esto le podriamos poner lo mismo que esp
    .esi = 0,
    .edi = 0,
    .es = GDT_IDX_DS_RING_0 << 3 | TI_0_RPL_0,
    .ss = GDT_IDX_DS_RING_0 << 3 | TI_0_RPL_0,
    .ds = GDT_IDX_DS_RING_0 << 3 | TI_0_RPL_0,
    .fs = GDT_IDX_DS_RING_0 << 3 | TI_0_RPL_0,
    .gs = GDT_IDX_DS_RING_0 << 3 | TI_0_RPL_0,
    .cs = GDT_IDX_CS_RING_0 << 3 | TI_0_RPL_0,
    .iomap = TSS_IOMAP_ALL_PORTS_DISABLED,
    .cr3= KERNEL_PAGE_DIR
    //los campos no definidos asumimos que estan con basura
};

tss_t tss_limpio = {
  .ss0 = GDT_IDX_DS_RING_0 << 3 | TI_0_RPL_0,
  .eip = TASK_CODE_VIRTUAL, //esta es la dire virtual de cualquier tarea lemming
  .esp = TASK_CODE_VIRTUAL + PAGE_SIZE*3, //tope de la pila
  .eflags = ENABLE_INT_FLAG,
  .eax = 0,
  .ecx = 0,
  .edx = 0,
  .ebx = 0,
  .ebp = 0,
  .esi = 0,
  .edi = 0,
  .es = GDT_IDX_DS_RING_3 << 3 | TI_0_RPL_3,
  .ss = GDT_IDX_DS_RING_3 << 3 | TI_0_RPL_3,
  .ds = GDT_IDX_DS_RING_3 << 3 | TI_0_RPL_3,
  .fs = GDT_IDX_DS_RING_3 << 3 | TI_0_RPL_3,
  .gs = GDT_IDX_DS_RING_3 << 3 | TI_0_RPL_3,
  .cs = GDT_IDX_CS_RING_3 << 3 | TI_0_RPL_3,
  .iomap = TSS_IOMAP_ALL_PORTS_DISABLED,
  //los campos no definidos asumimos que estan con basura
};

tss_t tss_tasks[10]; //arreglo de tss de todas las tareas de los lemmings

//termina de inicializar UNICAMENTE la tarea idle
void tss_init(void) {
  gdt[GDT_IDX_TSS_TAREA_IDLE].base_15_0 = (int)&tss_idle & 0xffff;
  gdt[GDT_IDX_TSS_TAREA_IDLE].base_23_16 = ((int)&tss_idle>>16) & 0xff;
  gdt[GDT_IDX_TSS_TAREA_IDLE].base_31_24 = (int)&tss_idle >> 24;
}

//inicializa la tss de nroTare con el mapeo en pd
void tss_task_init(uint8_t nroTarea, uint32_t pd){
  
  tss_tasks[nroTarea]=tss_limpio;

  tss_tasks[nroTarea].cr3 = pd;
  tss_tasks[nroTarea].esp0 = mmu_next_free_kernel_page() + PAGE_SIZE; //tope pila lvl 0

  gdt[GDT_IDX_TSS_TAREAS_BASE+nroTarea].base_15_0 = (int)&tss_tasks[nroTarea] & 0xffff;
  gdt[GDT_IDX_TSS_TAREAS_BASE+nroTarea].base_23_16 = ((int)&tss_tasks[nroTarea]>>16) & 0xff;
  gdt[GDT_IDX_TSS_TAREAS_BASE+nroTarea].base_31_24 = (int)&tss_tasks[nroTarea] >> 24;
}

void refresh_tss(uint32_t nroTarea){
  uint32_t cr3_task = tss_tasks[nroTarea].cr3;
  uint32_t esp0_task;
  if (tss_tasks[nroTarea].esp0 & 0xFFF)
  {
    esp0_task = (tss_tasks[nroTarea].esp0 & 0xFFFFF000) + PAGE_SIZE;
  }else{
    esp0_task = tss_tasks[nroTarea].esp0;
  }  

  tss_tasks[nroTarea]= tss_limpio;

  tss_tasks[nroTarea].cr3 = cr3_task;
  tss_tasks[nroTarea].esp0 = esp0_task;

  //test de refresh:
  //le hardcodeamos un valor cualquiera a la tss para ver si el refresh se aplicaba o no. info tss dps de llamar al refresh.  
}

uint32_t dameCr3DeTss(void){
  uint32_t nroTarea = trAnroTarea();
  return tss_tasks[nroTarea].cr3;
}