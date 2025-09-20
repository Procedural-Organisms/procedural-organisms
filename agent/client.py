from osc4py3.as_eventloop import *

# definir funcion que toma como argumentos una direccion ip y dos puertos
# y crea 2 cientes de distintos nombres con distintos puertos para mandar mensajes osc
def client_startup(address: str = '127.0.0.1', port1: int = 57120):
    osc_startup()
    osc_udp_client(address, port1, 'scClient')
    print("OSC client started!")
    print(f"OSC client address: {address}:{port1}")



