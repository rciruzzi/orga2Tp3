/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Declaracion de las rutinas asociadas al juego.
*/

#ifndef __GAME_H__
#define __GAME_H__

#include "sched.h"
#include "i386.h"
#include "screen.h"
#include "mapa.h"


void game_tick(void);
void game_init(void);

typedef enum direction_e {
  DIR_UP = 0,
  DIR_RIGHT = 1,
  DIR_DOWN = 2,
  DIR_LEFT = 3
} direction_e;

typedef enum move_result_e {
  MOVE_RESULT_SUCCESS = 0,
  MOVE_RESULT_WALL = 1,
  MOVE_RESULT_LAKE = 2,
  MOVE_RESULT_BORDER = 3,
  MOVE_RESULT_LEMMING = 4
} move_result_e;

int coordenadas[10][2];
char mapaNivelInferior[40][80]; //antes estaba en mapa, lo movimos aca para correr los tests
uint32_t longevidad[10];

uint32_t nacerEnPantalla(int nroTarea);
void modificarAlrededores();
int max(int a,int b);
int min(int a,int b);
void matarLemmingPorCoordenada(int f,int c);
int buscarLemmingDeCoordenada(int f, int c);
void autodestruirseEnPantalla();
void matarLemmingDeCoordenadaYPantalla(int f,int c);
void bridge(direction_e direccion);
int leerTerminoJuego();
void hasWon();
void iniciar_mapa_nivel_inferior();
void destruirEnPantalla(int nroTarea);
void reemplazarXCasilleroDelMapaInferior(int f, int c);

void matarLemmingDeLongevidadYDesmapea(int nroTarea);
void nacerSiguiente(int equipo,int masLongevoIndice);
uint32_t maximoEnLongevidad(int equipo);
void matarYrevivirViejos(void);



#endif //  __GAME_H__
