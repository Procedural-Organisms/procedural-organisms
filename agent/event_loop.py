import time                         # crear funciones que usan clock de computadora
from osc4py3.as_eventloop import *

# definir funcion que toma como argumento una cantidad indefinida de funciones
# y las pone dentro de un event loop que tiene un limite de 1 ms
def event_loop(*tasks):
    while True:

        for a in tasks:
            a()

        osc_process()
        time.sleep(0.001)


