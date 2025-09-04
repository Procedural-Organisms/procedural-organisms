from random import random           # generar numeros aleatorios
from osc4py3 import oscbuildparse   # crear mensajes en formato osc
from osc4py3.as_eventloop import *  # mandar mensajes osc

import normalizer as norm

parametersVector = [None] * 6

# TODO crear clase que tome como argumentos:
# index, numero de parametros, funcion de cada parametro (cuyo resultado se guardaria
# en un vector respectivo al index del objeto), la direccion osc y los data types

# definir funciones que manden mensajes osc
def sendRightPerc():
    parametersVector[0] = 1
    parametersVector[1] = random()
    parametersVector[2] = random()
    trigger = parametersVector[0]
    rightPercCarfreq = norm.geo_minmax(parametersVector[1], 100.0, 5000.0)
    lowPercModfreq = norm.geo_minmax(parametersVector[2], 100.0, 5000.0)

    osc_send(oscbuildparse.OSCMessage(
        '/rightPerc', ',iff', [trigger, rightPercCarfreq, lowPercModfreq]   
    ),'scClient')


def sendLeftPerc():
    parametersVector[3] = 1
    parametersVector[4] = random()
    parametersVector[5] = random()
    trigger = parametersVector[3]
    leftPercCarfreq = norm.geo_minmax(parametersVector[4], 100.0, 5000.0)
    leftPercModfreq = norm.geo_minmax(parametersVector[5], 100.0, 5000.0)

    osc_send(oscbuildparse.OSCMessage(
        '/leftPerc', ',iff', [trigger, leftPercCarfreq, leftPercModfreq]   
    ),'scClient')
