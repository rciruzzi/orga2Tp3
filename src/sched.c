/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del scheduler
*/
#include "sched.h"


paddr_t direFisiCodigoTarea[2] = {0x18000,0x1A000};

void sched_init(void) {
  //creamos mapeo y tss (con su tss descriptor) para cada tarea
  for (uint8_t i = 0; i < 10; i=i+2)
  {
    //Amalin
    gdt[GDT_IDX_TSS_TAREAS_BASE+i] = gdt[GDT_IDX_TSS_TAREAS_BASE];
    paddr_t pd = mmu_init_task_dir(direFisiCodigoTarea[EQUIPO_AMA]);
    tss_task_init(i,pd);
    //Betarote
    gdt[GDT_IDX_TSS_TAREAS_BASE+i+1] = gdt[GDT_IDX_TSS_TAREAS_BASE];
    paddr_t pd2 = mmu_init_task_dir(direFisiCodigoTarea[EQUIPO_BETA]);
    tss_task_init(i+1,pd2);
  }
}

int nroTarea_a_borrar_tss = -1; //si es -1 no hay tarea pendiente por borrar su tss. Este 'flag' está porque no podemos borrar la tss de una tarea si la misma esta en ejecución.

uint32_t contadorCiclos = 0;

int equipoActual = 0; //0 es amalin, 1 es betarote

int nroTareaActual[2] = {-2,-1}; //guarda los indices de longevidad de las tareas ama(indice 0) y beta(indice 1)

uint16_t sched_next_task(void) {
  contadorCiclos++;
  chequearCiclos();

  equipoActual = (equipoActual + 1)%2; //equipo contrario
  nroTareaActual[equipoActual] = (nroTareaActual[equipoActual]+2)%10;
  
  //juega el equipo contrario al que jugó en el turno anterior?
  int cantPasadas = 0;
  while((longevidad[nroTareaActual[equipoActual]]==0) && (cantPasadas<4))
  {
    nroTareaActual[equipoActual] = (nroTareaActual[equipoActual]+2)%10;
    cantPasadas++;
  }
  //siguen jugando los del mismo equipo?
  cantPasadas = 0;
  if (longevidad[nroTareaActual[equipoActual]]==0) //la ultima que se ejecuto de los contrarios estaba viva?
  {
    equipoActual = (equipoActual + 1)%2; //equipo que estaba ejecutandose al entrar en sched_next_task
    nroTareaActual[equipoActual] = (nroTareaActual[equipoActual]+2)%10;
    while((longevidad[nroTareaActual[equipoActual]]==0) && (cantPasadas<4)){
      nroTareaActual[equipoActual] = (nroTareaActual[equipoActual]+2)%10;
      cantPasadas++;
    }
  }
  //si llegamos aca con 0 estan todos muertos
  if (longevidad[nroTareaActual[equipoActual]]==0)
  {
    return GDT_IDX_TSS_TAREA_IDLE;
  }else{
    return GDT_IDX_TSS_TAREAS_BASE + (uint16_t)nroTareaActual[equipoActual];
  }
}


//realiza lo pedido para 401 y 2001 ciclos
void chequearCiclos(void){
  if (contadorCiclos%401==0)
  {
    nacerSiguiente(EQUIPO_AMA,-2); //el -2 es para que refreshee la tss siempre
    nacerSiguiente(EQUIPO_BETA,-2);
  }
  if (contadorCiclos%2001==0)
  {
    matarYrevivirViejos();
  }
  else if(nroTarea_a_borrar_tss!=-1){//refresh postergado
    refresh_tss(nroTarea_a_borrar_tss);
    nroTarea_a_borrar_tss=-1; //para refreshear una sola vez si se postergó
  }
}

//recibe task register y devuelve nroTarea
uint32_t trAnroTarea(void){
  uint32_t tr = (uint32_t)rtr();
  uint32_t nroTarea = (tr/8)-GDT_IDX_TSS_TAREAS_BASE; //aca restabamos 3 al tr, pero hubo q sacarlo
  return nroTarea;
}