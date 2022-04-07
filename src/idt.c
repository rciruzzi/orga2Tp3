/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de las rutinas de atencion de interrupciones
*/

#include "idt.h"
#include "defines.h"
#include "game.h"
#include "i386.h"
#include "isr.h"
#include "screen.h"


idt_entry_t idt[255] = {0};

idt_descriptor_t IDT_DESC = {sizeof(idt) - 1, (uint32_t)&idt};

/*
    La siguiente es una macro de EJEMPLO para ayudar a armar entradas de
    interrupciones. Para usar, descomentar y completar CORRECTAMENTE los
    atributos y el registro de segmento. Invocarla desde idt_inicializar() de
    la siguiene manera:

    void idt_inicializar() {
        IDT_ENTRY(0);
        ...
        IDT_ENTRY(19);
        ...
    }
*/

#define ATTR_0_EXCEPTION 0x8E00
#define CS_RING_0 0x40
#define ATTR_3_EXCEPTION 0xEE00
#define CS_RING_3 0x48


#define IDT_ENTRY0(numero)                                                                           \
    idt[numero].offset_15_0 = (uint16_t) ((uint32_t)(&_isr ## numero) & (uint32_t) 0xFFFF);         \
    idt[numero].segsel = CS_RING_0;                                                                 \
    idt[numero].attr = ATTR_0_EXCEPTION;                                                            \
    idt[numero].offset_31_16 = (uint16_t) ((uint32_t)(&_isr ## numero) >> 16 & (uint32_t) 0xFFFF);


//el cs indica el nivel de proteccion con el que se ejecuta la interrupción mientras que el
//dpl indica el nivel de privilegio necesario para poder ejecutar la interrupción.
#define IDT_ENTRY3(numero)                                                                           \
    idt[numero].offset_15_0 = (uint16_t) ((uint32_t)(&_isr ## numero) & (uint32_t) 0xFFFF);         \
    idt[numero].segsel = CS_RING_0;                                                                 \
    idt[numero].attr = ATTR_3_EXCEPTION;                                                            \
    idt[numero].offset_31_16 = (uint16_t) ((uint32_t)(&_isr ## numero) >> 16 & (uint32_t) 0xFFFF);

void idt_init() { 
  IDT_ENTRY0(0);
  IDT_ENTRY0(1);
  IDT_ENTRY0(2);
  IDT_ENTRY0(3);
  IDT_ENTRY0(4);
  IDT_ENTRY0(5);
  IDT_ENTRY0(6);
  IDT_ENTRY0(7);
  IDT_ENTRY0(8);
  IDT_ENTRY0(9);
  IDT_ENTRY0(10);
  IDT_ENTRY0(11);
  IDT_ENTRY0(12);
  IDT_ENTRY0(13);
  IDT_ENTRY0(14);
  IDT_ENTRY0(16);
  IDT_ENTRY0(17);
  IDT_ENTRY0(18);
  IDT_ENTRY0(19);
  IDT_ENTRY0(32);
  IDT_ENTRY0(33);
  IDT_ENTRY3(88);
  IDT_ENTRY3(98);
  IDT_ENTRY3(108);
}

