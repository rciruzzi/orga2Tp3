# Test de memoria bajo demanda

**IMPORTANTE** Este test requiere que pongamos un fragmento de código en un
lugar, y que eso nunca trigeree un breakpoint.

Este test verifica que los lemmings no puedan acceder a la memoria del kernel
ni puedan acceder a la memoria fuera del area compartida.

El test tiene como prerequisito el test de «memoria/compartido» ya que usa la
memoria compartida para incrementar una variable.

## Instrucciones Previas

* Poner el siguiente bloque de código antes del popad en el handler del page fault.

```nasm
	mov eax, cr2
	cmp eax, 0x400000
	je .else
	xchg bx, bx ; Nunca deberiamos ejecutar esto
	nop
.else:
	; aca viene el popad
```

## Descripción del Test

Cada lemming usa la memoria compartida para incrementar un contador. Ese
contador le indica una página a la que debe acceder.  Primero intentan con la
página anterior a la primer página compartida, luego a la página siguiente a la
última página compartida, y luego empiezan a probar desde la página 0x0 hasta
la anterior a la primer página compartida. Todos estos accesos deberían generar
un page fault tal que la tarea deba ser desalojada. Si alguno de estos accesos
no genera un page fault, el Lemming se dirigirá hacia la posición en la
diagonal.

## Verificación de la funcionalidad pedida

Los breakpoints nunca deben ser alcanzados. Los lemmings nunca se deben mover
de la posición inicial (deben morir apenas empiezan).
