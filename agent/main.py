from osc4py3.as_eventloop import *
from osc4py3 import oscbuildparse

import client
import time_functions as tf
from event_loop import *


client.client_startup(port1= 11301, port2 = 11303)


lowPercTrigger = tf.LoopedFunction(
    lambda:
    osc_send(oscbuildparse.OSCMessage(
    '/lowPerc/trigger', ',i', [1]
    ),'scClient')
)

hiPercTrigger = tf.LoopedFunction(
    lambda:
    osc_send(oscbuildparse.OSCMessage(
    '/hiPerc/trigger', ',i', [1]
    ),'scClient')
)

leftFlashTrigger = tf.LoopedFunction(
    lambda:
    osc_send(oscbuildparse.OSCMessage(
    '/leftFlash/trigger', ',i', [1]
    ),'oglClient')
)

rightFlashTrigger = tf.LoopedFunction(
    lambda:
    osc_send(oscbuildparse.OSCMessage(
    '/rightFlash/trigger', ',i', [1]
    ),'oglClient')
)

try:
    event_loop(
        lambda: lowPercTrigger.loop_function(1.0/0.291),
        lambda: hiPercTrigger.loop_function(1.0/0.267),
        lambda: leftFlashTrigger.loop_function(1.0/0.291),
        lambda: rightFlashTrigger.loop_function(1.0/0.267)
    )
    
except KeyboardInterrupt:
    osc_terminate()
