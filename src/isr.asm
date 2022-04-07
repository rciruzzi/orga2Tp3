; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
;
; Definicion de rutinas de atencion de interrupciones


%include "print.mac"

%define IDLE_sel 112
%define GDT_IDX_TSS_TAREA_IDLE 0xE

BITS 32

sched_task_offset:     dd 0x0
sched_task_selector:   dw 0xFFFF

;; PIC
extern pic_finish1

;imprimir teclas
extern printScanCode
extern printNumero
extern imprimir_excepcion

;; Sched
extern sched_next_task

;Debug
extern imprimirDebug
extern guardarPantalla
extern recuperarPantalla
extern presionarY
extern tieneEC
extern leerModoDebug
extern trabarDebug

;genericas
extern dameCr3DeTss
extern trAnroTarea
extern autodestruirseEnPantalla
extern destruirEnPantalla

;desalojo tareas
extern matarLemmingDeLongevidadYDesmapea

;servicios
extern move
extern modificarAlrededores
extern bridge

;finalizacion juego
extern leerTerminoJuego

;memoria compartida
extern mapearMemoriaCompartida
;;

;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;
%macro ISR 1
global _isr%1

_isr%1:
    pushad
    
    ;Esto es de antes, imprimia la excepcion
    ; mov eax, %1
    ; push eax
    ; call imprimir_excepcion
    ; add esp, 4

    ;nos fijamos si es PF
    mov eax, %1
    cmp eax, 14
    jnz .noEsPF
    ;si estamos aca es porque es PF

    ;nos fijamos que no se pase del rango de memShared
    mov eax, cr2
    cmp eax, 0x13FFFFF
    jg .PFnoArreglable
    cmp eax, 0x400000 ;puede dar menos de 400000 por un page fault de protección
    jl .PFnoArreglable

    ;si estamos aca es porque el PF es arreglable
    push eax
    call mapearMemoriaCompartida
    add esp,4
   
    popad
    add esp,4
    iret

    .PFnoArreglable:
    .noEsPF:

    call autodestruirseEnPantalla

    call leerModoDebug
    cmp eax, 1
    jne .debugApagado
    
    ;aca debug prendido
    call trabarDebug    ;modo_debug = 2;  
    call guardarPantalla

    ;imprimir panel debug
    push %1
    call tieneEC
    add esp, 4
    
    cmp eax, 0
    jne .hayEC
    ;si caemos aca es porque no tiene errorCode
    popad               ;agregamos error code justo dps de lo stackeado por el cambio de privilegio
    push 0xDEADBEEF     ;para respetar aridad de imprimirDebug
    pushad
    
    .hayEC:
    call dameCr3DeTss
    ;pusheo de lo que falta para imprimirDebug
    mov ecx, cr0
    push ecx
    mov ecx, cr2
    push ecx
    push eax ;push cr3
    mov ecx, cr4
    push ecx
    push %1
    call imprimirDebug ;no restauramos la pila para el iret porque aca no volvemos mas

    .debugApagado:
        desalojarTarea

    ;no deberíamos entrar acá nunk
    popad
    jmp $ 

%endmacro

%macro desalojarTarea 0
    call trAnroTarea
    push eax ;nroTarea actual

    ;matar lemming actual por excepción/explode/bridge
    call matarLemmingDeLongevidadYDesmapea
    add esp,4
    
    ;imprime reloj de tarea
    call next_clock
    ;pasamos a idle
    jmp IDLE_sel:0x0
%endmacro




;; Rutina de atención de las EXCEPCIONES
;; -------------------------------------------------------------------------- ;;
ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 16
ISR 17
ISR 18
ISR 19
ISR 20
ISR 21

;; Rutina de atención del RELOJ
;; -------------------------------------------------------------------------- ;;
global _isr32

_isr32:
    pushad
    ;Avisar al pic que se recibió la interrupción
    call pic_finish1
    call next_clockSistema

    ;fin juego?
    call leerTerminoJuego
    cmp eax,1
    jz .noHacerNadaFinJuego

    ;debug trabado?
    call leerModoDebug
    cmp eax, 2
    je .noIntercambioTask

    call sched_next_task ;devuelve indice en gdt de la task

    ;ahora transformamos indice a selector
    shl ax, 3
    
    ;TEST ejec beta2
    ; cmp ax, 18*8
    ; jne .seguirSinBK
    ; ;xchg bx,bx
    ; .seguirSinBK:    

    ;Imprimir el reloj de la tarea
    call next_clock
    
    ;si estoy ejecutando la tarea que devuelve el scheduler no salto
    str cx
    cmp cx, ax
    je .noIntercambioTask ;seguir ejecutando tarea actual(idle o lemming)

    ;el salto
    mov [sched_task_selector], ax
    jmp far [sched_task_offset]

    .noIntercambioTask:
    .noHacerNadaFinJuego:
    popad
    iret

;; Rutina de atención del TECLADO
;; -------------------------------------------------------------------------- ;;

global _isr33

;Estan comentados los prints de teclas. Descomentar para restaurar esta funcionalidad
_isr33:
    pushad
    call pic_finish1
    xor eax, eax
    ;leemos tecla presionada
    in al, 0x60

    cmp al, 0x15
    je .esY

    cmp al, 0x01
    je .noEsNumero
    
    cmp al, 0x0B
    ja .noEsNumero ; jump sin signo
    push eax
    ;call printNumero  Comentamos el print para que no imprima en pantalla durante el juego
    add esp, 4
    jmp .finish

.noEsNumero:
    push eax
    ;call printScanCode Comentamos el print para que no imprima en pantalla durante el juego
    add esp, 4
    jmp .finish
    
    .esY:
        call leerModoDebug
        cmp eax, 2
        ;si pasamos de 2 a 1 debemos restaurar el juego
        jne .noRestauro
        ;si era 2 recuperamos la pantalla del juego, pasamos de 2 a 1 para que la int_reloj destrabe el debug
        call recuperarPantalla
        .noRestauro:
            call presionarY
        .finish:
            popad
            iret
;; Rutinas de atención de las SYSCALLS
;; -------------------------------------------------------------------------- ;;

global _isr88
_isr88:
    pushad
    ;TEST de ejec beta2
    ; str cx
    ; cmp cx, 18*8
    ; jne .noEsBETA2
    ; xchg bx,bx
    ; .noEsBETA2:

    push eax ;direccion movimiento
    call move
    add esp,4
    mov [esp+7*4],eax ;el que popeamos despues va a ser la respuesta del move

    mov eax, 0x58 ;quedo de los primeros ejercicios

    call next_clock

    jmp IDLE_sel:0x0
    popad
    iret

global _isr98
_isr98:
    pushad
    ; mov eax, 0x62

    call modificarAlrededores

    desalojarTarea

    popad
    iret

global _isr108
_isr108:
    pushad
    ; mov eax, 0x6c

    add esp,28  ;7*4, esp apunta a eax que llego al servicio
    call bridge
    sub esp,28

    desalojarTarea

    popad
    iret




;; Funciones Auxiliares
;; -------------------------------------------------------------------------- ;;



baseTaskClockState:           times 10 dd 0
isrClock:            db '|/-\'

next_clock:
        pushad

        xor eax, eax
        str eax                 ;selector de tss
        ;sub eax, 3              ;le sacamos el rpl
        shr eax, 3              ;índice de gdt de tss
        sub eax, 15             ;nroTarea
        mov edi, eax            ;guardamos el nroTarea
        cmp edi, 0
        jl .esIdleOinit

        inc DWORD [baseTaskClockState + 4*edi]
        mov ebx, [baseTaskClockState + 4*edi]

        mov ecx, 2
        xor edx, edx
        div ecx                 ;dividimos nroTarea para saber si es A o B
        mov eax, edi            ;guardamos el nroTarea en eax de nuevo
        cmp edx, 0
        je .amalin
        .betarote:  
                mov esi, 40 ;los clocks de betarote van apartir de aca
                sub edi, 1  
                shl edi, 1  ;entre cada clock hay 4 casilleros
                add esi, edi  
                jmp .print

        .amalin:
                mov esi, 18 ;los clocks de amalin van apartir de aca
                shl edi, 1
                add esi, edi
        
        .print:
        cmp ebx, 0x4
        jl .ok
                mov DWORD [baseTaskClockState + 4*eax], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0f, 43, esi
                .esIdleOinit:
                    popad
        ret

isrNumberSistema:           dd 0x00000000
next_clockSistema:
        pushad
        inc DWORD [isrNumberSistema]
        mov ebx, [isrNumberSistema]
        cmp ebx, 0x4
        jl .ok
                mov DWORD [isrNumberSistema], 0x0
                mov ebx, 0
        .ok:
                add ebx, isrClock
                print_text_pm ebx, 1, 0x0f, 49, 79
                popad
        ret