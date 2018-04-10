# -*- coding: utf-8 -*-


import math
import random

"""
Dado x en [0,1) dar su representacion en binario, por ejemplo
dec2bin(0.625)='101'
dec2bin(0.0625)='0001'

Dada la representación binaria de un real perteneciente al intervalo [0,1) 
dar su representación en decimal, por ejemplo

bin2dec('101')=0.625
bin2dec('0001')=0.0625

nb número máximo de bits

"""



def dec2bin(x,nb=100):
    bin = ''
    num = x
    i = -1
    while num != 0 and i >= -100:
        pow_num = pow(2,i)
        if num >= pow_num:
            num -= pow_num
            bin += '1'
        else:
            bin += '0'
        i -= 1
    return bin


def bin2dec(xb):
    i = -1
    num = 0
    for b in xb:
        if b == '1':
            num += pow(2, i)
        i -= 1
    return num


"""
Dada una distribución de probabilidad p(i), i=1..n,
hallar su función distribución:
f(0)=0
f(i)=sum(p(k),k=1..i).
"""

def cdf(p):
    f = [0]
    accum = 0
    for n in p:
        accum += n
        f.append(accum)
    return f




"""
Dado un mensaje y su alfabeto con su distribución de probabilidad
dar el intervalo (l,u) que representa al mensaje.

mensaje='ccda'
alfabeto=['a','b','c','d']
probabilidades=[0.4,0.3,0.2,0.1]
Arithmetic(mensaje,alfabeto,probabilidades)=0.876 0.8776
"""

def Arithmetic(msg, alf, p):
    F = cdf(p)
    m = 0
    M = 1
    for c in msg:
        old_m = m
        m = m + (M - m) * F[alf.index(c)]
        M = old_m + (M - old_m) * F[alf.index(c) + 1]
    return m, M


"""
Dado un mensaje y su alfabeto con su distribución de probabilidad
dar la representación binaria de x/2**(t) siendo t el menor
entero tal que 1/2**(t)<l-u, r entero (si es posible par) tal
que m*2**(t)<= x <M*2**(t)

mensaje='ccda'
alfabeto=['a','b','c','d']
probabilidades=[0.4,0.3,0.2,0.1]
EncodeArithmetic1(mensaje,alfabeto,probabilidades)='111000001'
"""

def EncodeArithmetic1(msg, alf, p):
    m, M = Arithmetic(msg, alf, p)
    if M - m == 0:
        raise ValueError('Precision error with message: ' + msg)
    t = -math.floor(math.log(M-m, 2))
    x = math.ceil(m*pow(2,t))
    if x % 2 == 1 and x+1 < M*pow(2, t):
        x += 1
    return dec2bin(x/pow(2, t))



"""
Dado un mensaje y su alfabeto con su distribución de probabilidad
dar el código que representa el mensaje obtenido a partir de la
representación binaria de M y m

mensaje='ccda'
alfabeto=['a','b','c','d']
probabilidades=[0.4,0.3,0.2,0.1]
EncodeArithmetic2(mensaje,alfabeto,probabilidades)='111000001'

"""

def EncodeArithmetic2(msg, alf, p):
    m, M = Arithmetic(msg, alf, p)
    m_bin = dec2bin(m)
    M_bin = dec2bin(M)
    ans = ''
    for a, b in zip(m_bin, M_bin):
        if a == b:
            ans += a
        else:
            break
    if len(M_bin) != len(ans) + 1:
        return ans + '1'
    idx = len(ans)
    if idx < len(m_bin):
        ans += '0'
        while idx < len(m_bin) - 1 and m_bin[idx] == '1':
            ans += '1'
    return ans


"""
Dada la representación binaria del número que representa un mensaje, la
longitud del mensaje y el alfabeto con su distribución de probabilidad
dar el mensaje original

code='0'
longitud=4
alfabeto=['a','b','c','d']
probabilidades=[0.4,0.3,0.2,0.1]
DecodeArithmetic(code,longitud,alfabeto,probabilidades)='aaaa'

code='111000001'
DecodeArithmetic(code,4,alfabeto,probabilidades)='ccda'
DecodeArithmetic(code,5,alfabeto,probabilidades)='ccdab'

"""

def DecodeArithmetic(code,n,alf,p):
    num = bin2dec(code)
    F = cdf(p)
    ans = ''
    for _ in range(n):
        i = 1
        while F[i] < num:
            i += 1
        ans += alf[i - 1]
        num = (num - F[i-1]) / (F[i] - F[i-1])
    return ans


'''
Función que compara la longitud esperada del
mensaje con la obtenida con la codificación aritmética
'''

def comparacion(mensaje,alfabeto,probabilidades):
    p=1.
    indice=dict([(alfabeto[i],i+1) for i in range(len(alfabeto))])
    for i in range(len(mensaje)):
        p=p*probabilidades[indice[mensaje[i]]-1]
    aux=-math.log(p,2), len(EncodeArithmetic1(mensaje,alfabeto,probabilidades)), len(EncodeArithmetic2(mensaje,alfabeto,probabilidades))
    print('Información y longitudes:',aux)
    return aux


'''
Generar 10 mensajes aleatorios de longitud 10<=n<=20 aleatoria
con las frecuencias esperadas 50, 20, 15, 10 y 5 para los caracteres
'a', 'b', 'c', 'd', 'e', codificarlo y compararlas longitudes
esperadas con las obtenidas.
'''

alfabeto=['a','b','c','d','e']
probabilidades=[0.5,0.2,0.15,0.1,.05]
U = 50*'a'+20*'b'+15*'c'+10*'d'+5*'e'
def rd_choice(X,k = 1):
    Y = []
    for _ in range(k):
        Y +=[random.choice(X)]
    return Y

l_max=20

for _ in range(10):
    n=random.randint(10,l_max)
    L = rd_choice(U, n)
    mensaje = ''
    for x in L:
        mensaje += x
    print('---------- ',mensaje)
    C=comparacion(mensaje,alfabeto,probabilidades)
    print(C)


'''
Generar 10 mensajes aleatorios de longitud 10<=n<=100 aleatoria
con las frecuencias esperadas 50, 20, 15, 10 y 5 para los caracteres
'a', 'b', 'c', 'd', 'e' y codificarlo.
'''
# alfabeto=['a','b','c','d','e']
# probabilidades=[0.5,0.2,0.15,0.1,.05]
# U = 50*'a'+20*'b'+15*'c'+10*'d'+5*'e'
# def rd_choice(X,k = 1):
#     Y = []
#     for _ in range(k):
#         Y +=[random.choice(X)]
#     return Y
#
# l_max=100
#
# for _ in range(10):
#     n=random.randint(10,l_max)
#     L = rd_choice(U, n)
#     mensaje = ''
#     for x in L:
#         mensaje += x
#     print('---------- ',mensaje)
#     C = EncodeArithmetic1(mensaje,alfabeto,probabilidades)
#     print(C)

code='1110000101'
longitud=4
alfabeto=['a','b','c','d']
probabilidades=[0.4,0.3,0.2,0.1]
print(DecodeArithmetic(code,longitud,alfabeto,probabilidades))