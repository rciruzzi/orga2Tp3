/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones del manejador de memoria
*/

#include "mmu.h"
#include "i386.h"

#include "kassert.h"

unsigned int proxima_pagina_libre_kernel;
unsigned int proxima_pagina_libre_user;

//Si una tarea crea un mapeo, se registra aca para que cuando otra tarea del mismo equipo quiera acceder a la misma dire lineal lo haga a travez de la dire fisica que se mapeo.
//Cada pagina dire virtual se corresponde con un indice. Por ejemplo el indice 0 se corresponde con la pagina que empieza en 0x400000 y el 1 con 0x401000... hasta la 0x13ff000 con el indice 4095 (total de 4096 paginas) 
//estas estructuras las inicializamos con init_estructuras_memoria_compartida en kernel.asm
paddr_t* dirPhyMapeadasAmalin;
paddr_t* dirPhyMapeadasBetarote; // ocupan 4 marcos de pagina cada uno


//Al final vimos un test que nos muestra que la memoria compartida es persistente, por mas que muera todos los lemming de un equipo (memoria/invalido). Entonces sacamos esta parte:
//------------------------------------------
//EL indice representa la direccion lineal, y el valor dentro de cada elemento del arreglo indica cuantos lemmings estan usando el mapeo de esa direccion lineal en el equipo. (los valores van de 0 a 5)
//Le sumamos 1 cada vez que mapeamos la direccion lineal del equipo
//Le restamos 1 cada vez que un lemming con ese mapeo se muere. Para identificar si cada pagina esta mapeada en cada lemming, recorremos el arreglo de ese equipo y buscamos que direcciones no son cero. Para estas, buscamos en el mapeo del lemming que muere si ya estaba hecho mirando el bit de presente, y si estaba lo ponemos en cero, le restamos 1 a cuantasPaginasUsaban de esa direccion lineal y si llego a cero cuantasPaginasUsaban ponemos en arreglo de direcciones fisicas de ese equipo la entrada de esa direccion lineal con valor cero.
/*uint8_t* cuantosUsanPaginaAmalin;
uint8_t* cuantosUsanPaginaBetarote; *///ocupan 1 marco de pagina cada uno
//------------------------------------------


//Administrador de memoria
void mmu_init(void) {
	proxima_pagina_libre_kernel = INICIO_PAGINAS_LIBRES_KERNEL;
	proxima_pagina_libre_user = INICIO_PAGINAS_LIBRES_USER;
}


paddr_t mmu_init_task_dir(paddr_t phy_start){
	//los pd de las tareas van en el area libre del kernel
	page_directory_entry *pd = (page_directory_entry*)(mmu_next_free_kernel_page()); 

	//limpiamos page directory para que no nos tome mapeos anteriores ni basura como un mapeo que no queremos
 	for (int i = 0; i < 1024; ++i)
 	{
 		pd[i] =(page_directory_entry){0};
 	}

	//mapeo de kernel
	for (int i = 0; i < 1024; ++i)
	{
		mmu_map_page((uint32_t)pd, i*0x1000, i*0x1000, 3); //0 1 supervisor write
	}

	//mapeo primer pagina codigo tarea
	mmu_map_page((uint32_t)pd, TASK_CODE_VIRTUAL, phy_start, 5); // 1 0 user readonly
	//mapeo segunda pagina codigo tarea
	mmu_map_page((uint32_t)pd, TASK_CODE_VIRTUAL+0x1000, phy_start+0x1000, 5);  // 1 0 user readonly

	//mapeo pila de tarea
	paddr_t phy_pila = mmu_next_free_user_page();
	mmu_map_page((uint32_t)pd, TASK_STACK_BASE-0x1000, phy_pila, 7);  // atts: 1 1 user write , le restamos al tope inicial una pagina para apuntar al inicio de pagina de la pila
	
	return (paddr_t)pd;
}


paddr_t mmu_next_free_kernel_page(void) {
	unsigned int pagina_libre = proxima_pagina_libre_kernel;
	proxima_pagina_libre_kernel += PAGE_SIZE;
  return pagina_libre;
}

paddr_t mmu_next_free_user_page(void) {
	unsigned int pagina_libre = proxima_pagina_libre_user;
	proxima_pagina_libre_user += PAGE_SIZE;
  return pagina_libre;
}

paddr_t mmu_init_kernel_dir(void) {
	page_directory_entry *pd = (page_directory_entry*)KERNEL_PAGE_DIR;
	page_table_entry *pt_0 = (page_table_entry *)KERNEL_PAGE_TABLE_0;
	
	//Limpiamos entradas de directory y table
	for (int i = 0; i < 1024; ++i)
	{
		pd[i] = (page_directory_entry){0};
		pt_0[i] = (page_table_entry){0};
	}

	pd[0].present = MMU_FLAG_PRESENT;
	pd[0].user_supervisor = MMU_FLAG_SUPERVISOR;
	pd[0].read_write = MMU_FLAG_READWRITE;
	pd[0].page_table_base = ((paddr_t) pt_0) >> 12;
	

	for (int i = 0; i < 1024; ++i)
	{
		pt_0[i].present = MMU_FLAG_PRESENT;
		pt_0[i].user_supervisor = MMU_FLAG_SUPERVISOR;
		pt_0[i].read_write = MMU_FLAG_READWRITE;
		pt_0[i].physical_adress_base = i;
	}
  return (paddr_t)pd;
}

void mmu_map_page(uint32_t cr3, vaddr_t virt, paddr_t phy, uint32_t attrs){
	uint32_t directory_index = virt >> 22;
	uint32_t table_index = (virt >> 12) & 0x3FF;
	
	//obtenemos la direccion del directory entry
	page_directory_entry *pd = (page_directory_entry*)((cr3 & 0xFFFFF000));
	
	if (pd[directory_index].present!=1)
	 {
	 	page_table_entry *newpt = (page_table_entry*)mmu_next_free_kernel_page();
	 	//limpiamos la tabla
	 	for (int i = 0; i < 1024; ++i)
	 	{
	 		newpt[i] =(page_table_entry){0};
	 	}
	 	// cargamos en directory entry la direccion de tabla
	 	// los attrs van todos en 1 xq la misma directory entry puede mapear pags con diferentes attrs
	 	//queda el and entre los attributos
	 	pd[directory_index].present = (uint8_t)1;
		pd[directory_index].user_supervisor = (uint8_t)1;
		pd[directory_index].read_write = (uint8_t)1;
		pd[directory_index].page_table_base = ((paddr_t) newpt) >> 12;
 	 }
 	 //obtenemos la direccion del table entry
 	 page_table_entry *pt = (page_table_entry*)(pd[directory_index].page_table_base << 12); 
 	 //definimos el descriptor de pagina
 	 pt[table_index].present = (uint8_t)attrs;
	 pt[table_index].user_supervisor = (uint8_t)(attrs >> 2) ;
	 pt[table_index].read_write = (uint8_t)(attrs >> 1);
	 pt[table_index].physical_adress_base = ((paddr_t) phy) >> 12;

	 // los attrs quedan determinados por un and entre los attrs de la page entry y de la table entry

 	 tlbflush();
}

paddr_t mmu_unmap_page(uint32_t cr3, vaddr_t virt){
	uint32_t directory_index = virt >> 22;
	uint32_t table_index = (virt >> 12) & 0x3FF;
	paddr_t res;
	//obtenemos la direccion del directory entry
	page_directory_entry *pd = (page_directory_entry*)((cr3 & 0xFFFFF000));
	if (pd[directory_index].present==1)
	{
		//obtenemos la direccion del table entry
		page_table_entry *pt = (page_table_entry*)(pd[directory_index].page_table_base << 12);
		if (pt[table_index].present==1){
			vaddr_t page_offset = virt & 0x00000FFF;
			//devolvemos la direccion fisica que desmapeamos
			res=(pt[table_index].physical_adress_base << 12) + page_offset;
			//seteamos el bit de presente en 0
			pt[table_index].present = 0;
			tlbflush();
			return res;
		}else{
			return 0;
			tlbflush();
		}
	}
	return 0; //si desmapeo esa dir fisica, devuelve la dir fisica. sino cero
}


void mapearMemoriaCompartida(vaddr_t vadrAMapear){
	uint32_t cr3= dameCr3DeTss();
	int indiceDirLinealAMapear = (vadrAMapear>>12)-0x400;
	uint32_t nroTarea = trAnroTarea();
 	paddr_t phy;
	if((nroTarea%2)==EQUIPO_AMA){
		//amalin
		if(dirPhyMapeadasAmalin[indiceDirLinealAMapear]==0){
			//no esta mapeada
			phy=mmu_next_free_user_page();
			mmu_map_page(cr3,vadrAMapear& 0xFFFFF000,phy,0x7);
			for (int i = 0; i < 4096/4; ++i)
			{
				//siempre que agarro una porcion de memoria nueva esta todo el marco de pagina en cero.
				*((uint32_t*)(vadrAMapear+i))=0x0;
			}
			dirPhyMapeadasAmalin[indiceDirLinealAMapear]=phy;
		}else{
			//esta mapeada
			phy=dirPhyMapeadasAmalin[indiceDirLinealAMapear];
			mmu_map_page(cr3,vadrAMapear& 0xFFFFF000,phy,0x7);
		}
		//cuantosUsanPaginaAmalin[indiceDirLinealAMapear]++;
	}else{
		//betarote
		if(dirPhyMapeadasBetarote[indiceDirLinealAMapear]==0){
			//no esta mapeada
			phy=mmu_next_free_user_page();
			mmu_map_page(cr3,vadrAMapear& 0xFFFFF000,phy,0x7);
			for (int i = 0; i < 4096/4; ++i)
			{
				//siempre que agarro una porcion de memoria nueva esta todo el marco de pagina en cero.
				*((uint32_t*)(vadrAMapear+i))=0x0;
			}
			dirPhyMapeadasBetarote[indiceDirLinealAMapear]=phy;
		}else{
			//esta mapeada
			phy=dirPhyMapeadasBetarote[indiceDirLinealAMapear];
			mmu_map_page(cr3,vadrAMapear& 0xFFFFF000,phy,0x7);
		}
		//cuantosUsanPaginaBetarote[indiceDirLinealAMapear]++;
	}
}

void init_estructuras_memoria_compartida(){
	dirPhyMapeadasAmalin = (paddr_t*)mmu_next_free_kernel_page();
	proxima_pagina_libre_kernel+=3*PAGE_SIZE; //sumamos un marco de pagina, falta 3
	dirPhyMapeadasBetarote = (paddr_t*)mmu_next_free_kernel_page();
	proxima_pagina_libre_kernel+=3*PAGE_SIZE; 
	for (int i = 0; i < 8*PAGE_SIZE/4; ++i)
	{
		dirPhyMapeadasAmalin[i]=0x0;
	}
	//------------------------
	//VERSION ANTERIOR, la que desmapeaba si todos los lemming con ese mapeo morian:
	//cuantosUsanPaginaAmalin=(uint8_t*)mmu_next_free_kernel_page();
	//cuantosUsanPaginaBetarote=(uint8_t*)mmu_next_free_kernel_page();
	/*for (int i = 0; i < 10*PAGE_SIZE/4; ++i)
	{
		dirPhyMapeadasAmalin[i]=0x0;
	}*/
	//------------------------
}