from random import random           # generar numeros aleatorios
from osc4py3 import oscbuildparse   # crear mensajes en formato osc
from osc4py3.as_eventloop import *  # mandar mensajes osc

import normalizer as norm
import time_functions as tf

parametersVector = [None] * 24

# TODO crear clase que tome como argumentos:
# index, numero de parametros, funcion de cada parametro (cuyo resultado se guardaria
# en un vector respectivo al index del objeto), la direccion osc y los data types

# definir funciones que manden mensajes osc
def sendRightPerc():
    # TODO agregar tiempo en clock como primer parametro de vector
    parametersVector[0] = 1
    parametersVector[1] = random()
    parametersVector[2] = random()
    parametersVector[3] = random()
    parametersVector[4] = random()
    parametersVector[5] = random()

    trigger = parametersVector[0]
    rightPercCarfreq = norm.geo_minmax(parametersVector[1], 50.0, 1000.0)
    rightPercModfreq = norm.geo_minmax(parametersVector[2], 50.0, 1000.0)
    rightPercRelease = norm.minmax(parametersVector[4], 0.05, tf.period1)
    rightPercAttack = norm.minmax(parametersVector[3], 0.05, tf.period1 - rightPercRelease)
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
    leftPercCarfreq = norm.geo_minmax(parametersVector[7], 50.0, 1000.0)
    leftPercModfreq = norm.geo_minmax(parametersVector[8], 50.0, 1000.0)
    leftPercRelease = norm.minmax(parametersVector[10], 0.05, tf.period2)
    leftPercAttack = norm.minmax(parametersVector[9], 0.05, tf.period2 - leftPercRelease)
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


# ===  TEST  ===
def testLeft():
    parametersVector[12] = 1
    parametersVector[13] = parametersVector[1]
    parametersVector[14] = parametersVector[2]
    parametersVector[15] = parametersVector[3]
    parametersVector[16] = parametersVector[4]
    parametersVector[17] = parametersVector[5]

    trigger = parametersVector[12]
    leftPercCarfreq = norm.geo_minmax(parametersVector[13], 50.0, 1000.0)
    leftPercModfreq = norm.geo_minmax(parametersVector[14], 50.0, 1000.0)
    leftPercRelease = norm.minmax(parametersVector[16], 0.05, tf.period2)
    leftPercAttack = norm.minmax(parametersVector[15], 0.05, tf.period2 - leftPercRelease)
    leftPercCurve = norm.minmax(parametersVector[17], 0.05, tf.period2)

    osc_send(oscbuildparse.OSCMessage(
        '/testLeft', ',ifffff',
        [
            trigger,
            leftPercCarfreq,
            leftPercModfreq,
            leftPercAttack,
            leftPercRelease,
            leftPercCurve
        ] 
    ),'scClient')
    
def testRight():
    parametersVector[18] = 1
    parametersVector[19] = parametersVector[7]
    parametersVector[20] = parametersVector[8]
    parametersVector[21] = parametersVector[9]
    parametersVector[22] = parametersVector[10]
    parametersVector[23] = parametersVector[11]

    trigger = parametersVector[18]
    leftPercCarfreq = norm.geo_minmax(parametersVector[19], 50.0, 1000.0)
    leftPercModfreq = norm.geo_minmax(parametersVector[20], 50.0, 1000.0)
    leftPercRelease = norm.minmax(parametersVector[22], 0.05, tf.period2)
    leftPercAttack = norm.minmax(parametersVector[21], 0.05, tf.period2 - leftPercRelease)
    leftPercCurve = norm.minmax(parametersVector[23], 0.05, tf.period2)

    osc_send(oscbuildparse.OSCMessage(
        '/testRight', ',ifffff',
        [
            trigger,
            leftPercCarfreq,
            leftPercModfreq,
            leftPercAttack,
            leftPercRelease,
            leftPercCurve
        ]
    ), 'scClient')
# == == == == ==


# def sendRightFlash():
#     parametersVector[12] = 1
#     parametersVector[13] = parametersVector[3]
#     parametersVector[14] = parametersVector[4]

#     trigger = parametersVector[12]
#     rightFlashRelease = norm.minmax(parametersVector[14], 0.05, tf.period1)
#     rightFlashAttack = norm.minmax(parametersVector[13], 0.05, tf.period1 - rightFlashRelease)

#     osc_send(oscbuildparse.OSCMessage(
#         '/rightFlash', ',iff',
#         [
#             trigger,
#             rightFlashAttack,
#             rightFlashRelease,
#         ]   
#     ),'oglClient')


# def sendLeftFlash():
#     parametersVector[15] = 1
#     parametersVector[16] = parametersVector[9]
#     parametersVector[17] = parametersVector[10]

#     trigger = parametersVector[15]
#     leftFlashRelease = norm.minmax(parametersVector[17], 0.05, tf.period2)
#     leftFlashAttack = norm.minmax(parametersVector[16], 0.05, tf.period2 - leftFlashRelease)

#     osc_send(oscbuildparse.OSCMessage(
#         '/leftFlash', ',iff',
#         [
#             trigger,
#             leftFlashAttack,
#             leftFlashRelease,
#         ]   
#     ),'oglClient')

