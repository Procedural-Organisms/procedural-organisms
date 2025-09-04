import math

def geo_minmax(x, min = 0.0, max = 1.0):
    return min * ((max / min) ** x)

def minmax(x, min = 0.0, max = 1.0):
    return x * (max - min) + min

