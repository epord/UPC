# -*- coding: utf-8 -*-

import random

print("== START 1.1 ==")

'''
0. Dada una codificación R, construir un diccionario para codificar m2c y otro para decodificar c2m
'''
R = [('a','0'), ('b','11'), ('c','100'), ('d','1010'), ('e','1011')]

# encoding dictionary
m2c = dict(R)

# decoding dictionary
c2m = dict([(c,m) for m, c in R])


'''
1. Definir una función Encode(M, m2c) que, dado un mensaje M y un diccionario 
de codificación m2c, devuelva el mensaje codificado C.
'''
def Encode(M, m2c):
    C = ''
    for c in M:
        C += m2c[c]
    return C
    
    
''' 
2. Definir una función Decode(C, c2m) que, dado un mensaje codificado C y un diccionario 
de decodificación c2m, devuelva el mensaje original M.
'''
def Decode(C, c2m):
    if C in c2m.keys():
        return c2m[C]

    possible_keys = [key for key, value in c2m.items() if C.startswith(key)]
    if len(possible_keys) == 0:
        return None

    for k in possible_keys:
        res = Decode(C[len(k):], c2m)
        if res != None:
            return c2m[k] + res

#------------------------------------------------------------------------
# Ejemplo 1
#------------------------------------------------------------------------

R = [('a','0'), ('b','11'), ('c','100'), ('d','1010'), ('e','1011')]

# encoding dictionary
m2c = dict(R)

# decoding dictionary
c2m = dict([(c,m) for m, c in R])

'''
3. Generar un mensaje aleatorio M de longitud 50 con las frecuencias 
esperadas 50, 20, 15, 10 y 5 para los caracteres
'a', 'b', 'c', 'd', 'e' y codificarlo.
'''
M = ''.join([random.choice('aaaaaaaaaabbbbcccdde') for _ in range(50)])
C = Encode(M,m2c)


''' 
4. Si 'a', 'b', 'c', 'd', 'e' se codifican inicialmente con un código de 
bloque de 3 bits, hallar la ratio de compresión al utilizar el nuevo código.  
'''
r = 1/3


#------------------------------------------------------------------------
# Ejemplo 2
#------------------------------------------------------------------------
R = [('a','0'), ('b','10'), ('c','110'), ('d','1110'), ('e','1111')]

# encoding dictionary
m2c = dict(R)

# decoding dictionary
c2m = dict([(c,m) for m, c in R])

''' 
5.
Codificar y decodificar 20 mensajes aleatorios de longitudes también aleatorios.  
Comprobar si los mensajes decodificados coinciden con los originales.
'''

def test_codification(n, max_message_length):
    for _ in range(n):
        M = ''.join([random.choice('abcde') for _ in range(random.randint(1,max_message_length))])
        C = Encode(M,m2c)
        M2 = Decode(C, c2m)
        if M != M2:
            print('Random codification/decodification process failed!')
            print('Conflict was \'' + M + '\'\n')
            return
    print('Random codification/decodification process succeed!')

test_codification(20, 100)



#------------------------------------------------------------------------
# Ejemplo 3 
#------------------------------------------------------------------------
R = [('a','0'), ('b','01'), ('c','011'), ('d','0111'), ('e','1111')]

# encoding dictionary
m2c = dict(R)

# decoding dictionary
c2m = dict([(c,m) for m, c in R])

''' 
6. Codificar y decodificar los mensajes  'ae' y 'be'. 
Comprobar si los mensajes decodificados coinciden con los originales.
'''

M1 = 'ae'
M2 = 'be'

C1 = Encode(M1, m2c)
C2 = Encode(M2, m2c)

D1 = Decode(C1, c2m)
D2 = Decode(C2, c2m)

print(M1 + ' -> ' + C1 + ' -> ' + D1)
print(M2 + ' -> ' + C2 + ' -> ' + D2)


print("== END 1.1 ==")

