from random import random           # generar numeros aleatorios
from osc4py3 import oscbuildparse   # crear mensajes en formato osc
from osc4py3.as_eventloop import *  # mandar mensajes osc

import normalizer as norm
import time_functions as tf

parametersVector = [None] * 12

# TODO crear clase que tome como argumentos:
# index, numero de parametros, funcion de cada parametro (cuyo resultado se guardaria
# en un vector respectivo al index del objeto), la direccion osc y los data types

# definir funciones que manden mensajes osc
def sendRightPerc():
    parametersVector[0] = 1
    parametersVector[1] = random()
    parametersVector[2] = random()
    parametersVector[3] = random()
    parametersVector[4] = random()
    parametersVector[5] = random()

    trigger = parametersVector[0]
    rightPercCarfreq = norm.geo_minmax(parametersVector[1], 100.0, 5000.0)
    rightPercModfreq = norm.geo_minmax(parametersVector[2], 100.0, 5000.0)
    rightPercAttack = norm.minmax(parametersVector[3], 0.05, tf.period1)
    rightPercRelease = norm.minmax(parametersVector[4], 0.05, tf.period1)
    rightPercCurve = norm.minmax(parametersVector[5], 0.05, tf.period1)

    osc_send(oscbuildparse.OSCMessage(
        '/rightPerc', ',ifffff',
        [
            trigger,
            rightPercCarfreq,
            rightPercModfreq,
            rightPercAttack,
            rightPercRelease,
            rightPercCurve
        ]   
    ),'scClient')


def sendLeftPerc():
    parametersVector[6] = 1
    parametersVector[7] = random()
    parametersVector[8] = random()
    parametersVector[9] = random()
    parametersVector[10] = random()
    parametersVector[11] = random()

    trigger = parametersVector[6]
    leftPercCarfreq = norm.geo_minmax(parametersVector[7], 100.0, 5000.0)
    leftPercModfreq = norm.geo_minmax(parametersVector[8], 100.0, 5000.0)
    leftPercAttack = norm.minmax(parametersVector[9], 0.05, tf.period2)
    leftPercRelease = norm.minmax(parametersVector[10], 0.05, tf.period2)
    leftPercCurve = norm.minmax(parametersVector[11], 0.05, tf.period2)

    osc_send(oscbuildparse.OSCMessage(
        '/leftPerc', ',ifffff',
        [
            trigger,
            leftPercCarfreq,
            leftPercModfreq,
            leftPercAttack,
            leftPercRelease,
            leftPercCurve
        ]  
    ),'scClient')
