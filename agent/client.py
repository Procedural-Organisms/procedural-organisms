from osc4py3.as_eventloop import *

def client_startup(address: str = '127.0.0.1', port1: int = 57120, port2: int = 57121):
    osc_startup()
    osc_udp_client(address, port1, 'scClient')
    osc_udp_client(address, port2, 'oglClient')



