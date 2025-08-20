from osc4py3.as_eventloop import *

def client_startup(address: str = '127.0.0.1', port: int = 57120):
    osc_startup()
    osc_udp_client(address, port, 'client')



