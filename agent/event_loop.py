import time
from osc4py3.as_eventloop import *

def event_loop(*tasks):
    while True:

        for a in tasks:
            a()

        osc_process()
        time.sleep(0.001)


