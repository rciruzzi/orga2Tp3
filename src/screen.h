/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de funciones del scheduler
*/

#ifndef __SCREEN_H__
#define __SCREEN_H__

/* Definicion de la pantalla */
#define VIDEO_FILS 50
#define VIDEO_COLS 80

#include "colors.h"
#include "defines.h"
#include "stdint.h"
#include "sched.h"
#include "i386.h"

/* Estructura de para acceder a memoria de video */
typedef struct ca_s {
  uint8_t c;
  uint8_t a;
} ca;

void print(const char* text, uint32_t x, uint32_t y, uint16_t attr);
void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y,
               uint16_t attr);
void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y,
               uint16_t attr);

void screen_draw_box(uint32_t fInit, uint32_t cInit, uint32_t fSize,
                     uint32_t cSize, uint8_t character, uint8_t attr);

void screen_draw_layout(void);

void guardarPantalla(void);

void recuperarPantalla(void);

void imprimirDebug(uint32_t nroExcepcion, uint32_t cr4, uint32_t cr3, uint32_t cr2, uint32_t cr0, uint32_t edi, uint32_t esi, uint32_t ebp, uint32_t espInt, uint32_t ebx, uint32_t edx, uint32_t ecx, uint32_t eax, uint32_t err, uint32_t eip, uint32_t cs, uint32_t eflags,uint32_t espTarea, uint32_t ss);

uint32_t lemmingsSpawneados[2];  // indices 0: amalin 1:betarote

void actualizarPanelSpawneados(int equipo);

char* descripcionExcepcion[22];

void imprimir_excepcion(int codigo);

void printNumero(int codigo);

#endif //  __SCREEN_H__
