# -*- coding: utf-8 -*-
"""

"""
import math
import numpy as np
# import matplotlib.pyplot as plt


print("== START 1.3 ==")

'''
Dada una lista p, decidir si es una distribución de probabilidad (ddp)
0<=p[i]<=1, sum(p[i])=1.
'''


def es_ddp(p, tolerancia=10 ** (-5)):
    accum = 0
    for n in p:
        accum += n
        if n > 1 or n < 0:
            return False
    return abs(accum - 1) < tolerancia


'''
Dado un código C y una ddp p, hallar la longitud media del código.
'''


def LongitudMedia(C, p):
    l = 0
    for a, b in zip(C, p):
        l += len(a) * b
    return l


'''
Dada una ddp p, hallar su entropía.
'''


def H1(p):
    entropy = 0
    for n in p:
        if n != 0:
            entropy += n * np.log2(1.0 / n)
    return entropy


'''
Dada una lista de frecuencias n, hallar su entropía.
'''


def H2(n):
    total = sum(n)
    entropy = 0
    for f in n:
        if f != 0:
            p = f / total
            entropy += p * np.log2(1.0 / p)
    return entropy


'''
Ejemplos
'''
C = ['001', '101', '11', '0001', '000000001', '0001', '0000000000']
p = [0.5, 0.1, 0.1, 0.1, 0.1, 0.1, 0]
n = [5, 2, 1, 1, 1]

print(H1(p))
print(H2(n))
print(LongitudMedia(C, p))

'''
Dibujar H(p,1-p)
'''


'''
Hallar aproximadamente el máximo de  H(p,q,1-p-q)
'''



print("== END 1.3 ==")


