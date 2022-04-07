; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================

;paginacion
extern mmu_init
extern mmu_init_kernel_dir
extern imprimir_libretas
extern mmu_init_task_dir

;pic
extern pic_enable
extern pic_reset
;interrupciones 
extern idt_init
extern IDT_DESC
;segmento
extern GDT_DESC
;Adress GDT
%define CS_RING_0 64
%define CS_RING_3 72
%define DS_RING_0 80
%define DS_RING_3 88
;selectores de tareas
%define INIT_sel 104
%define IDLE_sel 112
%define tarea0_sel 120


;funciones de tss
extern tss_init
extern tss_task_init

;funciones de sched
extern sched_init

;inicializar pantalla juego
extern printearObstaculosIniciales

;inicializar estructuras memshared
extern init_estructuras_memoria_compartida

;inicializar mapa niv inferior para tests
extern iniciar_mapa_nivel_inferior

;pintar pantalla 
%define ancho_panel_puntaje 10

;pila
%define EBP_INIT 151552
%define ESP_INIT 151552
;macros
%include "print.mac"


global start


BITS 16
;; Saltear seccion de datos
jmp start

;;
;; Seccion de datos.
;; -------------------------------------------------------------------------- ;;

start_rm_msg db     'Iniciando kernel en Modo Real'
start_rm_len equ    $ - start_rm_msg

start_pm_msg db     'Iniciando kernel en Modo Protegido'
start_pm_len equ    $ - start_pm_msg
;dibujar pantalla
pantallaVacia times 4000 db ' '
pantLength equ $ - pantallaVacia
pantLength2 equ pantLength-800 

amalin db 'Amalin  00  01  02  03  04'
amalinLength equ $ - amalin
betarote db '00  01  02  03  04  Betarote'
betaroteLength equ $ - betarote

puntajeRojo db '  000000  '
puntajeAzul db '  000000  '


;;
;; Seccion de código.
;; -------------------------------------------------------------------------- ;;

;; Punto de entrada del kernel.
BITS 16
start:
    ; Deshabilitar interrupciones
    cli

    ; Cambiar modo de video a 80 X 50
    mov ax, 0003h
    int 10h ; set mode 03h
    xor bx, bx
    mov ax, 1112h
    int 10h ; load 8x8 font

    ;Imprimir mensaje de bienvenida
    print_text_rm start_rm_msg, start_rm_len, 0x07, 0, 0

    ;prueba acceder arriba del 1er MB
    ;mov ax, [CS:0x100001]
    ; Habilitar A20
    call A20_enable

    ; Cargar la GDT

    lgdt [GDT_DESC]

    ; Setear el bit PE del registro CR0

    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    
    ; Saltar a modo protegido

    jmp CS_RING_0:modo_protegido

BITS 32

modo_protegido:

    
    ; Establecer selectores de segmentos

    mov ax, DS_RING_0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Establecer la base de la pila
    
    mov ebp, EBP_INIT
    mov esp, ESP_INIT
    ; Imprimir mensaje de bienvenida

    print_text_pm start_pm_msg, start_pm_len, 0x07, 0, 0

    ; Inicializar pantalla

    ;limpiarPantalla
    print_text_pm pantallaVacia, pantLength, 0x00, 0, 0

    ;pinta la pantalla de acuerdo a lo pedido
    pintarPantalla

    ;Inicializamos pantalla del juego (8a) )
    call printearObstaculosIniciales
    
    ; Inicializar el manejador de memoria
    
    call mmu_init
 
    ; Inicializar el directorio de paginas
    call mmu_init_kernel_dir
    
    ; Cargar directorio de paginas
    push eax
    mov cr3, eax

    ; Habilitar paginacion
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    

    ;TEST probamos mapa de memoria de tareas
    ; push 0x18000
    ; call mmu_init_task_dir
    ; mov cr3, eax
    ; add esp, 4
    ;restauramos mapa de memoria al kernel    
    ; pop eax 
    ; mov cr3, eax
    

    call imprimir_libretas
    
    ; Cargar tarea inicial
    mov ax, INIT_sel
    ltr ax 

    ; Inicializar tss de la tarea Idle
    call tss_init

    ;TEST intercambio a tarea0
    ; push 0x1A000
    ; call mmu_init_task_dir
    ; push eax
    ; push 0
    ; call tss_task_init
    ; jmp tarea0_sel:0x0
    
    ; Inicializar el scheduler, incluidas las tss
    call sched_init
    
    ; Inicializar la IDT

    call idt_init
    
    ; Cargar IDT
    lidt [IDT_DESC]
    
    ; Configurar controlador de interrupciones
    call pic_reset
    call pic_enable


    ;TEST prueba de excepciones por software
    ;inc esp
    ;movdqa xmm0, [ss:esp]
    
    ;xor eax, eax
    ;idiv eax ;division por 0
    
    ;iniciamos mapa niv inferior de manera dinamica para que cargue el mapa de los tests
    call iniciar_mapa_nivel_inferior

    ;iniciamos los arreglos de memoria compartida
    call init_estructuras_memoria_compartida
    
    ; Habilitar interrupciones
    sti

    ; Saltar a la primera tarea: Idle
    jmp IDLE_sel:0x0
    
    ; Ciclar infinitamente (por si algo sale mal...)
    mov eax, 0xFFFF
    mov ebx, 0xFFFF
    mov ecx, 0xFFFF
    mov edx, 0xFFFF
    jmp $

;; -------------------------------------------------------------------------- ;;

%include "a20.asm"



