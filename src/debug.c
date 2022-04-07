#include "debug.h"

int modo_debug = 0; //0: modo debug apagado, 1:modo debug y tareas corriendo, 2:modo debug y idle en loop

void trabarDebug(void){
  modo_debug = 2;
}

int leerModoDebug(void){
  return modo_debug;
}

void presionarY(void){
  if (modo_debug==0)
  {
    //prender debug
  	print("DEBUG ACTIVO, ESPERANDO EXCEPCION",0,49,0x0e);
    modo_debug++;
  }else{
    //apagar debug
  	if(modo_debug==1){
  		print("                                 ",0,49,0x0f);
  	}
    modo_debug--;
  }
}