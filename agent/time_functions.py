import time # importar funciones de tiempo

period1 = 1/0.291
period2 = 1/0.267

# clase con metodo que repite funcion cada cierta cantidad de tiempo si esta se encuentra dentro de un loop
class LoopedFunction:
    def __init__(self, functionToLoop):
        # elapsedSeconds calcula la cantidad de segundos desde que se inicio la funcion
        # startTime calcula la hora a la que se inicio la funcion
        # y newCycle le dice a la funcion si esta lista para ejecutar la subfuncion
        # estas lineas solo se ejecutan cuando se crea la instance de la clase
        self.elapsedSeconds = 0.0
        self.startTime = time.perf_counter()
        self.functionArmed = True
        self.functionToLoop = functionToLoop

    # metodo que repite una funcion cada cierta cantidad de tiempo
    def loop_function(self, cycleDuration = 1.0):
        # en cuanto el modulo entre los segundos transcurridos y la duracion del ciclo es mayor que 0 esto dispara
        # la subfuncion y le dice a la funcion que espere hasta la segunda mitad del ciclo para alistarse.
        # pero como la funcion esta escrita para que solo se pueda activar en la primera mitad entonces esta
        # espera a que el ciclo se complete antes de volver a disparar la subfuncion
        if (0.5 > (self.elapsedSeconds % cycleDuration) / cycleDuration > 0.0) and (self.functionArmed == True):
            self.functionToLoop()
            self.functionArmed = False
        if ((self.elapsedSeconds % cycleDuration) / cycleDuration > 0.5) and (self.functionArmed == False):
            self.functionArmed = True

        # actualizar tiempo transcurrido desde que se inicio la funcion
        self.elapsedSeconds = time.perf_counter() - self.startTime
