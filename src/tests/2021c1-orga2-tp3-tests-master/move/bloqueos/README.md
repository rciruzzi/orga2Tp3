# Test de Bloqueos.

Este test consiste en un mapa que tiene paredes y lagos.  Las tareas Amarin y
Betarote van a ir a los lugares donde hay paredes, lagos y los bordes de la
pantalla y chequear que el resultado de la syscall move sean correctos.

En caso de fallar, estas tareas van a generar un breakpoint (`xchg bx, bx`).

Una vez que terminen de chequear los obstaculos, los lemmings van a ir al centro
de la pantalla para chocarse con el lemming del equipo opuesto. Cuando esto sucede,
ambos cometen una excepción para ser desalojados.
