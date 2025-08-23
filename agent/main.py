from osc4py3.as_eventloop import *
from osc4py3 import oscbuildparse

import client
import time_functions as tf
from event_loop import *


client.client_startup(port= 11302)


lowPercTrigger = tf.LoopedFunction(lambda:
                                osc_send(oscbuildparse.OSCMessage(
                                    '/lowPerc/trigger', ',i', [1]
                                    ),'client')
                                )

hiPercTrigger = tf.LoopedFunction(lambda:
                               osc_send(oscbuildparse.OSCMessage(
                                   '/hiPerc/trigger', ',i', [1]
                                   ),'client')
                               )


try:
    event_loop(
        lambda: lowPercTrigger.loop_function(1.0/0.291),
        lambda: hiPercTrigger.loop_function(1.0/0.267)
    )
    
except KeyboardInterrupt:
    osc_terminate()
