from osc4py3.as_eventloop import *

def print_handler(a):
    print(f'OSC message received: {a}')

osc_startup()
osc_udp_server('127.0.0.1', 11300, 'server')

osc_method('/hello', print_handler)

finished = False
while not finished:
    osc_process()

osc_terminate()
