# Test de memoria compartida.

Este test verifica que los lemmings puedan compartir memoria.  Lo primero que
hacen, es acceder a la primer dirección del area de memoria compartida. Esta
siempre debe tener 0 (no se inicializa con basura).  Luego, al arrancar, leen e
incrementan un contador, que les indica a qué posición deben ir.

Lo que deberíamos ver es a todos los lemmings parándose sobre la diagonal del
mapa.

**NOTA**: Como bonus, ademas se testea que luego de X ciclos los lemmings mas
viejos dejan el juego y dan lugar a lemmings nuevos.
