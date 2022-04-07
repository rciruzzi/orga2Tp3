# Test básico de memoria.

Este test revisa condiciones básicas de memoria de cada Lemming:

* El código arranca en la posición virtual esperada.
* El stack arranca en la posición virtual esperada.
* Todas las páginas (codigo x 2 + stack) son legibles.
* La primer pagina de código no es legible.

## Qué esperar
Primero se realizan los chequeos no intrusivos, en caso de que esos fallen, ocurrirá un breakpoint.
Una vez se verifique que las 3 páginas son legibles, el lemming se moverá en dirección al centro de la pantalla.
Cuando avance un poco, va a tratar de escribir en la memoria de la primer página de código. Lo que se espera es que el Lemming muera ahí.

Si el Lemming se queda trabado, significa que el código no está mapeado como solo lectura.
