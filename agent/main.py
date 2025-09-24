from osc4py3.as_eventloop import *  # mandar mensajes osc
from osc4py3 import oscbuildparse   # crear mensajes en formato osc

import client
import time_functions as tf
import osc_messages as oscm
from event_loop import *

client.client_startup(port1= 13933)

# TODO comenzar transmicion de osc hasta que se inicie el video

# crear objetos de clase LoopedFuntion
# que toman una funcion a loopear como argumeto
# e introduzco una funcion lamda que crea y manda mensajes osc
rightPercLooped = tf.LoopedFunction(
    lambda: oscm.sendRightPerc() 
)

leftPercLooped = tf.LoopedFunction(
    lambda: oscm.sendLeftPerc()
)

leftColorLooped = tf.LoopedFunction(
    lambda: oscm.sendLeftColor()
)
rightColorLooped = tf.LoopedFunction(
    lambda: oscm.sendRightColor()
)

try:
    # llamar a la funcion event_loop
    # que toma como argumento una cantidad indeterminada de funciones
    # para ejecutar en el event loop
    event_loop(
        lambda: rightPercLooped.loop_function(tf.period1),
        lambda: leftPercLooped.loop_function(tf.period2),
        lambda: leftColorLooped.loop_function(tf.period1),
        lambda: rightColorLooped.loop_function(tf.period2),
    )

# TODO limpiar recursos con cualquier tipo de salida del programa
# limpiar recursos con ctrl + c    
except KeyboardInterrupt:
    osc_terminate()
