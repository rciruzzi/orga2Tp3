# Test de memoria bajo demanda

**IMPORTANTE** Este test requiere que pongamos breakpoints en dos lugares y
verifiquemos manualmente en bochs las cosas.

Este test verifica que los lemmings puedan mapear memoria bajo demanda.  El
test depende del test de memoria compartida. Si no implementan memoria
compartida, probablemente este test no funcione.  Basicamente cada lemming va
accediendo una a una a las páginas del area compartida, y estas se deben mapear
bajo demanda.

## Instrucciones Previas

* Poner un breakpoint antes del `iret` del `isr14` (o como sea que se llame la
  función que usan para el handler de page fault).

## Descripción del Test

Cada Lemming debe generar DOS page faults en el area bajo demanda, luego se
mueve yendo a la diagonal, como en el test de «memoria/compartido» El primer
page fault lo hace para acceder a la primer pagina compartida, que tiene un
contador que va incrementando cada Lemming. El segundo acceso es a la iésima
página (que se obtiene tras leer el valor de la pagina compartida e
incrementarlo. 

En la pantalla deberíamos ver el mismo comportamiento que en el test de
«memoria/compartido», solo que además, deberíamos verificar mediante bochs que
todo ande bien.

## Verificación de la funcionalidad pedida

Con los breakpoints insertados previamente, deberiamos verlos en pares de
irets.

Cuando estemos en el iret, hacer creg para ver los registros de control. El cr2
debería apuntar a la página compartida, el primer iret a la primera y el
segundo a la que se va incrementando. Luego de ejecutar `creg`, ejecutar `info
tab` para ver que efectivamente se están mapeando.

Si vamos viendo que el Lemming se mueve, significa que los page faults se
resolvieron bien.
