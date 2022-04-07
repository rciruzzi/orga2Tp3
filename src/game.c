/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
*/

#include "game.h"
#include "mapa.h"
#include "types.h"

int coordenadas[10][2]={{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}};

uint32_t terminoJuego=0;

uint32_t longevidad[10] = {0,0,0,0,0,0,0,0,0,0}; //si es 0 es porque el lemming esta muerto, caso contrario el mínimo de cada paridad(de índices)representa al más longevo

char mapaNivelInferior[40][80];

//esta funcion se encarga de inicializar los campos que representan a un nuevo lemming
void nacerSiguiente(int equipo,int masLongevoIndice){
  for (int i = equipo; i < 10; i+=2)
  {
    if (longevidad[i]==0) //en 2001 va a encontrar a la que mataste recien
    {
      uint32_t nacio = nacerEnPantalla(i); 
      if(nacio==1){ 
        //si entró aca es que pudo nacer

        lemmingsSpawneados[equipo]++;

        //printear panel de lemmings spawneados
        actualizarPanelSpawneados(equipo);

        longevidad[i] = maximoEnLongevidad(equipo) + 1;

        // no se puede refreshear la tss de una tarea en ejecución en vez de perder un ciclo postergamos el refresh al siguiente.
        if ((equipoActual!=equipo) || (nroTareaActual[equipo]!=masLongevoIndice)) 
        {
          refresh_tss(i);
        }else{
          nroTarea_a_borrar_tss=masLongevoIndice; //postergamos refresh
        }
      }
      break; //no hace nacer a otros lemmings, solo al primero
    }
  }
}
//Nacer pantalla chequea si puede nacer un lemming en la posicion inicial, y si puede, lo imprime y guarda su coordenada
//devuelve si el nacimiento se produjo (1) o si fallo porque ya habia un lemming en esa posicion (0)
uint32_t nacerEnPantalla(int nroTarea){
	ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;
	if(nroTarea%2==EQUIPO_AMA){
		for (int i = 0; i < 10; ++i)
		{
			//ya habia un lemming en la posInit?
			if((longevidad[i]!=0) && ((coordenadas[i][0]==40) && (coordenadas[i][1]==0)) ){
				//hay un lemming en la posicion inicial y no hago nacer nada
				return 0; //no nacio
			}
		}
		//es amalin, nace
		coordenadas[nroTarea][0]=40;
		coordenadas[nroTarea][1]=0;
		//printeo de lemming que nace
		p[coordenadas[nroTarea][0]][coordenadas[nroTarea][1]].a=0x4f;
		p[coordenadas[nroTarea][0]][coordenadas[nroTarea][1]].c='A';
	}else{
		for (int i = 0; i < 10; ++i)
		{
			//ya habia un lemming en la posInit?
			if((longevidad[i]!=0) && ((coordenadas[i][0]==1) && (coordenadas[i][1]==79)) ){
				//hay un lemming en la posicion inicial y no hago nacer nada
				return 0; //no nacio
			}
		}
		//es beta, nace
		coordenadas[nroTarea][0]=1;
		coordenadas[nroTarea][1]=79;
		//printeo de lemming que nace
		p[coordenadas[nroTarea][0]][coordenadas[nroTarea][1]].a=0x1f;
		p[coordenadas[nroTarea][0]][coordenadas[nroTarea][1]].c='B';
	}
	return 1; //nacio
}

move_result_e move(direction_e direccion){
	ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;
	uint32_t nroTarea= trAnroTarea(); //nroTarea actual
	int  queSumoACoordParaVolver[2] ={0,0};  //en caso de que te quieras mover a una pared, un lago o a otro lemming
	//muevo (cambio coordenadas)
	switch(direccion){
		case DIR_UP:
			coordenadas[nroTarea][0]--;
			queSumoACoordParaVolver[0]=1;
			break;
		case DIR_RIGHT:
			coordenadas[nroTarea][1]++;
			queSumoACoordParaVolver[1]=-1;
			break;
		case DIR_DOWN:
			coordenadas[nroTarea][0]++;
			queSumoACoordParaVolver[0]=-1;
			break;
		case DIR_LEFT:
			coordenadas[nroTarea][1]--;
			queSumoACoordParaVolver[1]=1;
			break;
	}
	// movimiento fuera de rango?
	if(coordenadas[nroTarea][0]==0 || coordenadas[nroTarea][0]==41 || coordenadas[nroTarea][1]==-1 || coordenadas[nroTarea][1]==80 ){
		coordenadas[nroTarea][0]+=queSumoACoordParaVolver[0];
		coordenadas[nroTarea][1]+=queSumoACoordParaVolver[1];
		return MOVE_RESULT_BORDER;
	}
	//chequeo constante de victoria
	hasWon();

	//La coordenada vuelve a donde estaba si se topo con un lemming, una pared o un lago.
	//Si se logra mover, reemplaza donde estaba y printea a donde va
	switch(p[coordenadas[nroTarea][0]][coordenadas[nroTarea][1]].c){
		//se queda en donde estaba:
		case 'P':
			coordenadas[nroTarea][0]+=queSumoACoordParaVolver[0];
			coordenadas[nroTarea][1]+=queSumoACoordParaVolver[1];
			return MOVE_RESULT_WALL;
		case 'L':
			coordenadas[nroTarea][0]+=queSumoACoordParaVolver[0];
			coordenadas[nroTarea][1]+=queSumoACoordParaVolver[1];
			return MOVE_RESULT_LAKE;
		case 'A':
			coordenadas[nroTarea][0]+=queSumoACoordParaVolver[0];
			coordenadas[nroTarea][1]+=queSumoACoordParaVolver[1];
			return MOVE_RESULT_LEMMING;
		case 'B':
			coordenadas[nroTarea][0]+=queSumoACoordParaVolver[0];
			coordenadas[nroTarea][1]+=queSumoACoordParaVolver[1];
			return MOVE_RESULT_LEMMING;
		//se mueve:
		case 'X':
		case '+':
		case '.':	
			//printear lemming en posicion hacia la que te moves
			if((nroTarea%2)==0){
				p[coordenadas[nroTarea][0]][coordenadas[nroTarea][1]].c='A' ;
				p[coordenadas[nroTarea][0]][coordenadas[nroTarea][1]].a= 0x4f;
			}else{
				p[coordenadas[nroTarea][0]][coordenadas[nroTarea][1]].c='B';
				p[coordenadas[nroTarea][0]][coordenadas[nroTarea][1]].a= 0x1f;
			}

			//printear como estaba en la posicion anterior del lemming
			int f = coordenadas[nroTarea][0]+queSumoACoordParaVolver[0];
			int c = coordenadas[nroTarea][1]+queSumoACoordParaVolver[1];
			reemplazarXCasilleroDelMapaInferior(f,c);
			return MOVE_RESULT_SUCCESS;
	}
	return 1234; //sino tira warning. no nos agarra todas las ramas del switch el compiler. Igual aca no llega
}

//explode
void modificarAlrededores(){
	ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;
	uint32_t nroTarea= trAnroTarea();
	int filInit =  coordenadas[nroTarea][0];
	int colInit =  coordenadas[nroTarea][1];
	//abajo
	int fil= min(filInit+1,40);
	int col=colInit;
	//es pared?
	if(p[fil][col].c=='P'){
		p[fil][col].c='X';
		mapaNivelInferior[fil-1][col]='X'; //guardamos escombro por si un lemming pasa por ahi
		p[fil][col].a=0x42;
	}else if ((p[fil][col].c=='A') || (p[fil][col].c=='B')) { //hay lemmings abajo?
		matarLemmingPorCoordenada(fil,col); //matamos no importa el equipo
	}

	//arriba
	fil= max(filInit-1,1);
	if(p[fil][col].c=='P'){
		p[fil][col].c='X';
		mapaNivelInferior[fil-1][col]='X';
		p[fil][col].a=0x42;
	}else if ((p[fil][col].c=='A') || (p[fil][col].c=='B')) { 
		matarLemmingPorCoordenada(fil,col);
	}

	//derecha
	fil=filInit;
	col= min(colInit+1,79);
	if(p[fil][col].c=='P'){
		p[fil][col].c='X';
		mapaNivelInferior[fil-1][col]='X'; 
		p[fil][col].a=0x42;
	}else if ((p[fil][col].c=='A') || (p[fil][col].c=='B')) { 
		matarLemmingPorCoordenada(fil,col);
	}

	//izquierda
	col= max(colInit-1,0);
	if(p[fil][col].c=='P'){
		p[fil][col].c='X';
		mapaNivelInferior[fil-1][col]='X';
		p[fil][col].a=0x42;
	}else if ((p[fil][col].c=='A') || (p[fil][col].c=='B')) {
		matarLemmingPorCoordenada(fil,col); 
	}

	//te matas a vos mismo siempre
	autodestruirseEnPantalla();
	//el lemming muere para el scheduler al desalojar la tarea en la int (macro)
}

void matarLemmingPorCoordenada(int f,int c){
	int nroTarea  = buscarLemmingDeCoordenada(f,c);
	matarLemmingDeLongevidadYDesmapea(nroTarea);
	reemplazarXCasilleroDelMapaInferior(f,c); // es lo mismo que destruirEnPantalla
}

//busca un lemming vivo en coordenadas especificadas, y devuelve su nroTarea
int buscarLemmingDeCoordenada(int f, int c){
	for (int i = 0; i < 10; ++i)
	{
		if((longevidad[i]!=0) && ((coordenadas[i][0]==f) && (coordenadas[i][1]==c))){
			return i;
		}
	}
	return -1; //aca nunca entra, esta solo por completitud. La coordenada que llega es siempre de un lemming vivo
}

void autodestruirseEnPantalla(){
	uint32_t nroTarea= trAnroTarea();
	int f =  coordenadas[nroTarea][0];
	int c =  coordenadas[nroTarea][1];
	reemplazarXCasilleroDelMapaInferior(f,c);
}

void destruirEnPantalla(int nroTarea){
	int f =  coordenadas[nroTarea][0];
	int c =  coordenadas[nroTarea][1];
	reemplazarXCasilleroDelMapaInferior(f,c);
}

//reemplaza en pantalla por lo que habia antes
void reemplazarXCasilleroDelMapaInferior(int f, int c){
	ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;	
	ca* punteroVideoPosAnterior=&p[f][c];
	switch(mapaNivelInferior[f-1][c]){ 
		case 'X': //escombro
			punteroVideoPosAnterior->c='X';
			punteroVideoPosAnterior->a=0x42;
		break;
		case '+':  //puente
			punteroVideoPosAnterior->c='+';
			punteroVideoPosAnterior->a=0x1f;	
		break;
		case '.':
			punteroVideoPosAnterior->c='.';
			punteroVideoPosAnterior->a=0x22;	
			break;
	}
}

void bridge(direction_e direccion){
	ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;
	uint32_t nroTarea= trAnroTarea();
	autodestruirseEnPantalla();
	switch(direccion){
		case DIR_UP:
			coordenadas[nroTarea][0]--;
			break;
		case DIR_RIGHT:
			coordenadas[nroTarea][1]++;
			break;
		case DIR_DOWN:
			coordenadas[nroTarea][0]++;
			break;
		case DIR_LEFT:
			coordenadas[nroTarea][1]--;
			break;
	}
	//en coordenadas queda la posicion el puente tentativo

	//esta fuera de rango?
	if(coordenadas[nroTarea][0]==0 || coordenadas[nroTarea][0]==41 || coordenadas[nroTarea][1]==-1 || coordenadas[nroTarea][1]==80 ){
		return;
	}

	//Hay agua en donde nos queremos mover?
	if(p[coordenadas[nroTarea][0]][coordenadas[nroTarea][1]].c=='L'){
		p[coordenadas[nroTarea][0]][coordenadas[nroTarea][1]].c='+';
		p[coordenadas[nroTarea][0]][coordenadas[nroTarea][1]].a=0x1f;
		//actualizamos mapaNivelInferior
		mapaNivelInferior[coordenadas[nroTarea][0]-1][coordenadas[nroTarea][1]]='+';
	}
	//el lemming muere para el scheduler al desalojar la tarea en la int(macro)
}

void hasWon(){
	uint32_t nroTarea= trAnroTarea();
	if(longevidad[nroTarea]!=0 && ((coordenadas[nroTarea][1]==79) && (nroTarea%2==EQUIPO_AMA))){
		//gano ama
		print("GANO AMALIN",40,0,0x0f);
		terminoJuego=1;
	}else if (longevidad[nroTarea]!=0 && ((coordenadas[nroTarea][1]==0) && (nroTarea%2==EQUIPO_BETA))){
		//gano beta
		print("GANO BETAROTE",40,0,0x0f);
		terminoJuego=1;
	}
}

int leerTerminoJuego(){
	return terminoJuego;
}

void iniciar_mapa_nivel_inferior(){
	for (int i = 0; i < 40; ++i)
	{
		for (int j = 0; j < 80; ++j)
		{
			mapaNivelInferior[i][j]=mapa_original[i][j];
		}
	}
}

void matarLemmingDeLongevidadYDesmapea(int nroTarea){
  longevidad[nroTarea] = 0; //el asesinato
  uint32_t cr3 = tss_tasks[nroTarea].cr3;
  for (uint32_t i = 0; i < 4096; ++i)
  {
    mmu_unmap_page(cr3,(i<<12)+0x400000);
    //------------------------
    //Version anterior que desmapeaba si morian todos los lemmings que lo tenian mapeado (memshared)
    /*paddr_t huboDesmapeo =*/ 
		/*if(huboDesmapeo!=0){
      if((nroTarea%2)==0){
        cuantosUsanPaginaAmalin[i]--;
        if(cuantosUsanPaginaAmalin[i]==0){
          dirPhyMapeadasAmalin[i]=0;
        }
      }else{
        cuantosUsanPaginaBetarote[i]--;
        if(cuantosUsanPaginaBetarote[i]==0){
          dirPhyMapeadasBetarote[i]=0;
        }
      }
    }*/
    //------------------------
  }
}


//esta funcion nos devuelve el valor que representa al lemming más joven del equipo, o sea, el máximo numérico
uint32_t maximoEnLongevidad(int equipo){
  uint32_t maxLong = 0;
  for (int i = equipo; i < 10; i+=2)
  {
    if (maxLong<longevidad[i])
    {
      maxLong = longevidad[i];
    }
  }
  return maxLong;
}

//si algun equipo tiene todos sus lemmings vivos mata al mas viejo y lo revive
void matarYrevivirViejos(void){
  int todosVivosAMA = 1;
  int todosVivosBETA = 1;
  int masLongevoAMAindice = 0;
  int masLongevoBETAindice = 1;

  //verificamos si estan todos vivos y encontramos el mas viejo de cada team
  for (int i = 0; i < 5; i++)
  {
    //amalin
    if (longevidad[masLongevoAMAindice]>longevidad[2*i])
    {
      masLongevoAMAindice = 2*i;
    }
    if (longevidad[2*i] == 0)
    {
      todosVivosAMA = 0;
    }
    //betarote
    if (longevidad[masLongevoBETAindice]>longevidad[2*i+1])
    {
      masLongevoBETAindice = 2*i+1;
    }
    if (longevidad[2*i+1] == 0)
    {
      todosVivosBETA = 0;
    }
  }
  //matar y revivir si corresponde
  if (todosVivosAMA)
  {
    matarLemmingDeLongevidadYDesmapea(masLongevoAMAindice);
    destruirEnPantalla(masLongevoAMAindice);
    nacerSiguiente(EQUIPO_AMA, masLongevoAMAindice);
  }
  if (todosVivosBETA)
  {
  	//TEST
    /*breakpoint();            //detectamos si se mata y revive
    __asm volatile("nop");*/
    matarLemmingDeLongevidadYDesmapea(masLongevoBETAindice);
    destruirEnPantalla(masLongevoBETAindice);
    nacerSiguiente(EQUIPO_BETA, masLongevoBETAindice);
  }
}


//Auxiliares

int max(int a,int b){
	if(a>b){
		return a;
	}else{
		return b;
	}
}
int min(int a,int b){
	if(a<b){
		return a;
	}else{
		return b;
	}
}