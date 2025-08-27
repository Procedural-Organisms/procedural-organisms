from osc4py3.as_eventloop import *

# handler function que determina que hacer cuando se recibe un mensaje osc
def print_handler(a):
    print(f'OSC message received: {a}')

# inicializacion osc server
osc_startup()
osc_udp_server('127.0.0.1', 11301, 'server')

# funcion que llama a la handler function cuando se recibe un mensaje
# en la direccion determinada
osc_method('/lowPerc/trigger', print_handler)

# event loop donde se llama la funcion que detecta recepcion de mensajes
finished = False
while not finished:
    osc_process()

osc_terminate()
