/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================

  Definicion de funciones de impresion por pantalla.
*/

#include "screen.h"

ca copia_pantalla[40][80]; //esto lo usamos para guardar y restaurar la pantalla (modo debug)

uint32_t lemmingsSpawneados[2] = {0,0};  //indices 0: amalin 1:betarote

int excepcionesConError[8] = {8,10,11,12,13,14,17,21};

char* descripcionExcepcion[22] ={
  "Divide Error #DE [0]",
  "Debug Exception #DB [1]",
  "NMI Interrupt [2]",
  "Breakpoint #BP [3]",
  "Overflow #OF [4]",
  "Bound Range Exceded #BR [5]",
  "Invalid Opcode #UD [6]",
  "Device Not Available #NM [7]",
  "Double Fault #DF [8]",
  "Coprocessor Segment Overrun [9]",
  "Invalid TSS #TS [10]",
  "Segment Not Present #NP [11]",
  "Stack Segment Fault #SS [12]",
  "General Protection #GP [13]",
  "Page Fault #PF [14]",
  "Reserved",
  "x87 FPU Floating Point Error #MF [16]",
  "Alignment Check #AC [17]",
  "Machine Check #MC [18]",
  "SIMD Floating Point Exception #XM [19]",
  "Virtualization Exception #VE [20]",
  "Control Protection Exception #CP [21]"};

int tieneEC(int nroExcepcion){
  for (int i = 0; i < 8; ++i)
  {
    if (excepcionesConError[i] == nroExcepcion)
    {
      return 1;
    }
  }
  return 0;
}


void printScanCode(uint8_t scanCode){
  if (!(scanCode & 0x80))
  {
    print_hex(scanCode, 2, 80/2 -1, 50/2, 0x0F);
 }
}

void print(const char* text, uint32_t x, uint32_t y, uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  int32_t i;
  for (i = 0; text[i] != 0; i++) {
    p[y][x].c = (uint8_t)text[i];
    p[y][x].a = (uint8_t)attr;
    x++;
    if (x == VIDEO_COLS) {
      x = 0;
      y++;
    }
  }
}

void print_dec(uint32_t numero, uint32_t size, uint32_t x, uint32_t y,
               uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  uint32_t i;
  uint8_t letras[16] = "0123456789";

  for (i = 0; i < size; i++) {
    uint32_t resto = numero % 10;
    numero = numero / 10;
    p[y][x + size - i - 1].c = letras[resto];
    p[y][x + size - i - 1].a = attr;
  }
}

void print_hex(uint32_t numero, int32_t size, uint32_t x, uint32_t y,
               uint16_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO; // magia
  int32_t i;
  uint8_t hexa[8];
  uint8_t letras[16] = "0123456789ABCDEF";
  hexa[0] = letras[(numero & 0x0000000F) >> 0];
  hexa[1] = letras[(numero & 0x000000F0) >> 4];
  hexa[2] = letras[(numero & 0x00000F00) >> 8];
  hexa[3] = letras[(numero & 0x0000F000) >> 12];
  hexa[4] = letras[(numero & 0x000F0000) >> 16];
  hexa[5] = letras[(numero & 0x00F00000) >> 20];
  hexa[6] = letras[(numero & 0x0F000000) >> 24];
  hexa[7] = letras[(numero & 0xF0000000) >> 28];
  for (i = 0; i < size; i++) {
    p[y][x + size - i - 1].c = hexa[i];
    p[y][x + size - i - 1].a = attr;
  }
}

void screen_draw_box(uint32_t fInit, uint32_t cInit, uint32_t fSize,
                     uint32_t cSize, uint8_t character, uint8_t attr) {
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;
  uint32_t f;
  uint32_t c;
  for (f = fInit; f < fInit + fSize; f++) {
    for (c = cInit; c < cInit + cSize; c++) {
      p[f][c].c = character;
      p[f][c].a = attr;
    }
  }
}

void guardarPantalla(void){
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;
  uint32_t f;
  uint32_t c;
  for (f = 1; f < 41; f++) {
    for (c = 0; c < 80; c++) {
      copia_pantalla[f-1][c].c = p[f][c].c;
      copia_pantalla[f-1][c].a = p[f][c].a;
    }
  }
}

void recuperarPantalla(void){
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;
  uint32_t f;
  uint32_t c;
  for (f = 1; f < 41; f++) {
    for (c = 0; c < 80; c++) {
      p[f][c].c = copia_pantalla[f-1][c].c;
      p[f][c].a = copia_pantalla[f-1][c].a;
    }
  }
}



void imprimirDebug(uint32_t nroExcepcion, uint32_t cr4, uint32_t cr3, uint32_t cr2, uint32_t cr0, uint32_t edi, uint32_t esi, uint32_t ebp, uint32_t espInt, uint32_t ebx, uint32_t edx, uint32_t ecx, uint32_t eax, uint32_t err, uint32_t eip, uint32_t cs, uint32_t eflags,uint32_t espTarea, uint32_t ss){
  screen_draw_box(1, 19, 40,40,(uint8_t)' ',0x02);
  
  //creamos un leax para testear que registros nos llegan.
  /* breakpoint();
  leax(espTarea);*/
  
  //imprimimos excepcion
  print(descripcionExcepcion[nroExcepcion], 20, 2, 0x02);
  uint32_t nroTarea = trAnroTarea();
  print_dec(nroTarea, 3, 55, 2, 0x02);
  //imprimimos tags de regs y flags
  int offset_fila = 6;
  int offset_columna = 21;
  print("eax", offset_columna, offset_fila, 0x0F);
  offset_fila+=2;
  print("ebx", offset_columna, offset_fila, 0x0F);
  offset_fila+=2;
  print("ecx", offset_columna, offset_fila, 0x0F);
  offset_fila+=2;
  print("edx", offset_columna, offset_fila, 0x0F);
  offset_fila+=2;
  print("esi", offset_columna, offset_fila, 0x0F);
  offset_fila+=2;
  print("edi", offset_columna, offset_fila, 0x0F);
  offset_fila+=2;
  print("ebp", offset_columna, offset_fila, 0x0F);
  offset_fila+=2;
  print("esp", offset_columna, offset_fila, 0x0F);
  offset_fila+=2;
  print("eip", offset_columna, offset_fila, 0x0F);
  offset_fila+=2;
  print("cs", offset_columna+1, offset_fila, 0x0F);
  offset_fila+=2;
  print("ds", offset_columna+1, offset_fila, 0x0F);
  offset_fila+=2;
  print("es", offset_columna+1, offset_fila, 0x0F);
  offset_fila+=2;
  print("fs", offset_columna+1, offset_fila, 0x0F);
  offset_fila+=2;
  print("gs", offset_columna+1, offset_fila, 0x0F);
  offset_fila+=2;
  print("ss", offset_columna+1, offset_fila, 0x0F);
  offset_fila+=3;
  print("eflags", offset_columna, offset_fila, 0x0F);

  //imprimimos tags de regs de control
  offset_fila=7;
  offset_columna=46;
  print("cr0", offset_columna, offset_fila, 0x0F);
  offset_fila+=2;
  print("cr2", offset_columna, offset_fila, 0x0F);
  offset_fila+=2;
  print("cr3", offset_columna, offset_fila, 0x0F);
  offset_fila+=2;
  print("cr4", offset_columna, offset_fila, 0x0F);
  offset_fila+=2;
  print("err", offset_columna, offset_fila, 0x0F);
  offset_fila+=3;
  offset_columna=40;
  print("stack", offset_columna, offset_fila, 0x0F);

  //imprimimos valores de regs y flags
  offset_fila = 6;
  offset_columna = 25;
  print_hex(eax,8, offset_columna, offset_fila, 0x02);
  offset_fila+=2;
  print_hex(ebx,8, offset_columna, offset_fila, 0x02);
  offset_fila+=2;
  print_hex(ecx,8, offset_columna, offset_fila, 0x02);
  offset_fila+=2;
  print_hex(edx,8, offset_columna, offset_fila, 0x02);
  offset_fila+=2;
  print_hex(esi,8, offset_columna, offset_fila, 0x02);
  offset_fila+=2;
  print_hex(edi,8, offset_columna, offset_fila, 0x02);
  offset_fila+=2;
  print_hex(ebp,8, offset_columna, offset_fila, 0x02);
  offset_fila+=2;
  print_hex(espTarea,8, offset_columna, offset_fila, 0x02);
  offset_fila+=2;
  print_hex(eip,8, offset_columna, offset_fila, 0x02);
  offset_fila+=2;
  print_hex(cs,8, offset_columna, offset_fila, 0x02);
  offset_fila+=2;
  print_hex(ss,8, offset_columna, offset_fila, 0x02);
  offset_fila+=2;
  print_hex(ss,8, offset_columna, offset_fila, 0x02);
  offset_fila+=2;
  print_hex(ss,8, offset_columna, offset_fila, 0x02);
  offset_fila+=2;
  print_hex(ss,8, offset_columna, offset_fila, 0x02);
  offset_fila+=2;
  print_hex(ss,8, offset_columna, offset_fila, 0x02);
  offset_fila+=3;
  print_hex(eflags,8, offset_columna+3, offset_fila, 0x02);

  //imprimimos valores de los regs de control
  offset_fila=7;
  offset_columna=50;
  print_hex(cr0,8, offset_columna, offset_fila, 0x02);
  offset_fila+=2;
  print_hex(cr2,8, offset_columna, offset_fila, 0x02);
  offset_fila+=2;
  print_hex(cr3,8, offset_columna, offset_fila, 0x02);
  offset_fila+=2;
  print_hex(cr4,8, offset_columna, offset_fila, 0x02);
  offset_fila+=2;
  print_hex(err,8, offset_columna, offset_fila, 0x02);

  //cuantos registros traemos de la pila de la tarea?
  offset_fila+=6;
  offset_columna = 40;
  //imprimimos desde el ultimo que pusheamos hasta 3 registros mas arriba en la pila
  if (espTarea==TASK_STACK_BASE)
  {
    print_hex(0xDEADBEEF,8, offset_columna, offset_fila, 0x02);
    offset_fila+=2;
    print_hex(0xDEADBEEF,8, offset_columna, offset_fila, 0x02);
    offset_fila+=2;
    print_hex(0xDEADBEEF,8, offset_columna, offset_fila, 0x02);
  }else if((espTarea%PAGE_SIZE)<=(PAGE_SIZE-12)){
    print_hex(*(uint32_t*)espTarea,8, offset_columna, offset_fila, 0x02);
    offset_fila+=2;
    print_hex(*(uint32_t*)(espTarea+4),8, offset_columna, offset_fila, 0x02);
    offset_fila+=2;
    print_hex(*(uint32_t*)(espTarea+8),8, offset_columna, offset_fila, 0x02);
  }else if ((espTarea%PAGE_SIZE)==(PAGE_SIZE-8))
  {
    print_hex(*(uint32_t*)espTarea,8, offset_columna, offset_fila, 0x02);
    offset_fila+=2;
    print_hex(*(uint32_t*)(espTarea+4),8, offset_columna, offset_fila, 0x02);
    offset_fila+=2;
    print_hex(0xDEADBEEF,8, offset_columna, offset_fila, 0x02);
  }else if ((espTarea%PAGE_SIZE)==(PAGE_SIZE-4))
  {
    print_hex(*(uint32_t*)espTarea,8, offset_columna, offset_fila, 0x02);
    offset_fila+=2;
    print_hex(0xDEADBEEF,8, offset_columna, offset_fila, 0x02);
    offset_fila+=2;
    print_hex(0xDEADBEEF,8, offset_columna, offset_fila, 0x02);
  }

  //le ponemos a espInt basura para que no se queje el compiler
  espInt++;
}

//imprime pantalla inicial del juego
void printearObstaculosIniciales(){
  ca(*p)[VIDEO_COLS] = (ca(*)[VIDEO_COLS])VIDEO;
  uint32_t f;
  uint32_t c;
  for (f = 1; f < 41; f++) {
    for (c = 0; c < 80; c++) {
      //TEST un Amelin, no tenemos impl logica de nacer todavia
      /*if((f==41) && (c==1) ){ 
        p[f][c].a = 0x4f;
        p[f][c].a ='A';
      }*/
      p[f][c].c = mapa_original[f-1][c];
      if(p[f][c].c == 'P' ){
        p[f][c].a = 0x4C;
      }
      else if(p[f][c].c == 'L' ){
        p[f][c].a = 0x11;
      }
      else if(p[f][c].c == '.' ){
        p[f][c].a = 0x22;
      }
    }
  }
}

void actualizarPanelSpawneados(int equipo){
  if(equipo==0){
    //nacio uno de amalin
    print_dec(lemmingsSpawneados[equipo],6,12,45,0x4f);
  }else{
    //nacio uno de betarote
    print_dec(lemmingsSpawneados[equipo],6,62,45,0x1f);
  }
}

void imprimir_libretas(void){
  print("619/18  197/20  228/17", 0, 0, 0x20);
}

void imprimir_excepcion(int codigo){
    print(descripcionExcepcion[codigo], 0, 0, 0x0A);
}

void printNumero(int codigo){
    if (codigo == 11)
    {
      print_dec(0,1, 79, 0, 0x0A);

    }else{
      codigo--;
      print_dec(codigo,1,79,0,0x0A);
    }
}