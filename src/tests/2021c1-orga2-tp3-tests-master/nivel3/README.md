# Test tareas corren en nivel 3

Este test es para verificar si las tareas efectivamente están corriendo en
nivel 3 o están en nivel 0.

Para ello, se trata de leer del registro cr0, instrucción solo permitida en CPL=0.
Luego, las tareas van hacia el centro de la pantalla.

Si el test es exitoso, no deberíamos ver ninguna tarea moviéndose, ya que deberían
morirse por generar una excepción.
