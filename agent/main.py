from osc4py3.as_eventloop import *  # mandar mensajes osc
from osc4py3 import oscbuildparse   # crear mensajes en formato osc

import client
import time_functions as tf
import osc_messages as oscm
from event_loop import *

# creo 2 clientes con distintos puertos
# para transmitir a video-generator y audio-generator
client.client_startup(port1= 11301, port2 = 11303)

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

leftFlashTrigger = tf.LoopedFunction(
    lambda:
    osc_send(oscbuildparse.OSCMessage(
        '/leftFlash/trigger', ',i', [1]
    ),'oglClient')
)

rightFlashTrigger = tf.LoopedFunction(
    lambda:
    osc_send(oscbuildparse.OSCMessage(
        '/rightFlash/trigger', ',i', [1]
    ),'oglClient')
)


# crear una forma de limpiar recursos con ctrl + c 
try:
    # llamar a la funcion event_loop
    # que toma como argumento una cantidad indeterminada de funciones
    # para ejecutar en el event loop
    event_loop(
        # metodos para repetir argumentos dentro de objetos Looped_Function
        lambda: rightPercLooped.loop_function(tf.period1),
        lambda: leftPercLooped.loop_function(tf.period2),
        lambda: leftFlashTrigger.loop_function(tf.period1),
        lambda: rightFlashTrigger.loop_function(tf.period2)
    )
# TODO limpiar recursos con cualquier tipo de salida del programa
# limpiar recursos con ctrl + c    
except KeyboardInterrupt:
    osc_terminate()
