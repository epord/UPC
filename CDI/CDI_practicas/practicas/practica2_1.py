# -*- coding: utf-8 -*-
"""

"""

from practicas.practica1_1 import *
from practicas.practica1_3 import *


print("== START 2.1 ==")

#%%----------------------------------------------------

'''
Dada una distribucion de probabilidad, hallar un código de Huffman asociado
'''
def code_rec(tree, code, prefix):
    # Left branch
    if not isinstance(tree[1], list):
        code.append(prefix + '1')
    else:
        code_rec(tree[1], code, prefix + '1')

    # Right branch
    if not isinstance(tree[0], list):
        code.append(prefix + '0')
    else:
        code_rec(tree[0], code, prefix + '0')



def Huffman(p):
    if p == [1]:
        return ['0']
    # Init
    tree = []
    code = []
    for prob in p:
        tree.append([prob, prob])
    # Making tree
    while len(tree) != 1:
        tree.sort(key=lambda x: x[0])
        min1 = tree[0]
        min2 = tree[1]
        new_node = [min1[0] + min2[0], [min1[1], min2[1]]]
        tree = [new_node] + tree[2:]
    # Code from tree
    tree = tree[0][1]
    code_rec(tree, code, '')
    return code

#%%----------------------------------------------------

'''
Dada la ddp p=[0.80,0.1,0.05,0.05], hallar un código de Huffman asociado,
la entropía de p y la longitud media de código de Huffman hallado.
'''

p=[0.80,0.1,0.05,0.05]
code = Huffman(p)
print("Huffman :" + str(code))
print("Entropia: " + str(H1(p)))
print("Longitud media: " + str(LongitudMedia(code, p)))


#%%----------------------------------------------------

'''
Dada la ddp p=[1/n,..../1/n] con n=2**8, hallar un código de Huffman asociado,
la entropía de p y la longitud media de código de Huffman hallado.
'''

n=2**2
p=[1/n for _ in range(n)]
code = Huffman(p)
print("Huffman :" + str(code))
print("Entropia: " + str(H1(p)))
print("Longitud media: " + str(LongitudMedia(code, p)))


#%%----------------------------------------------------

'''
Dado un mensaje hallar la tabla de frecuencia de los caracteres que lo componen
'''
def tablaFrecuencias(mensaje):
    tabla = {}
    for c in mensaje:
        tabla[c] = tabla.get(c, 0) + 1
    return tabla


# '''
# Definir una función que codifique un mensaje utilizando un código de Huffman
# obtenido a partir de las frecuencias de los caracteres del mensaje.
#
# Definir otra función que decodifique los mensajes codificados con la función
# anterior.
# '''
#%%----------------------------------------------------

def EncodeHuffman(msg):
    length = len(msg)
    frec = tablaFrecuencias(msg)
    letras = []
    p = []

    for k, v in frec.items():
        letras.append(k)
        p.append(v/length)

    code = Huffman(p)

    m2c = {}
    for l, c in zip(letras, code):
        m2c[l] = c

    mensaje_codificado = Encode(msg, m2c)

    return mensaje_codificado, m2c
    
    
def DecodeHuffman(msg,m2c):
    c2m = {v: k for k, v in m2c.items()}
    ans = ''
    while msg != '':
        for k in c2m.keys():
            if msg.startswith(k):
                ans += c2m[k]
                msg = msg[len(k):]
                break
    return ans
        
#%%----------------------------------------------------

'''
Ejemplo
'''
mensaje                 = 'La heroica ciudad dormía la siesta. El viento Sur, caliente y perezoso, empujaba las nubes blanquecinas que se rasgaban al correr hacia el Norte. En las calles no había más ruido que el rumor estridente de los remolinos de polvo, trapos, pajas y papeles que iban de arroyo en arroyo, de acera en acera, de esquina en esquina revolando y persiguiéndose, como mariposas que se buscan y huyen y que el aire envuelve en sus pliegues invisibles. Cual turbas de pilluelos, aquellas migajas de la basura, aquellas sobras de todo se juntaban en un montón, parábanse como dormidas un momento y brincaban de nuevo sobresaltadas, dispersándose, trepando unas por las paredes hasta los cristales temblorosos de los faroles, otras hasta los carteles de papel mal pegado a las esquinas, y había pluma que llegaba a un tercer piso, y arenilla que se incrustaba para días, o para años, en la vidriera de un escaparate, agarrada a un plomo. Vetusta, la muy noble y leal ciudad, corte en lejano siglo, hacía la digestión del cocido y de la olla podrida, y descansaba oyendo entre sueños el monótono y familiar zumbido de la campana de coro, que retumbaba allá en lo alto de la esbeltatorre en la Santa Basílica. La torre de la catedral, poema romántico de piedra,delicado himno, de dulces líneas de belleza muda y perenne, era obra del siglo diez y seis, aunque antes comenzada, de estilo gótico, pero, cabe decir, moderado por uninstinto de prudencia y armonía que modificaba las vulgares exageraciones de estaarquitectura. La vista no se fatigaba contemplando horas y horas aquel índice depiedra que señalaba al cielo; no era una de esas torres cuya aguja se quiebra desutil, más flacas que esbeltas, amaneradas, como señoritas cursis que aprietandemasiado el corsé; era maciza sin perder nada de su espiritual grandeza, y hasta sussegundos corredores, elegante balaustrada, subía como fuerte castillo, lanzándosedesde allí en pirámide de ángulo gracioso, inimitable en sus medidas y proporciones.Como haz de músculos y nervios la piedra enroscándose en la piedra trepaba a la altura, haciendo equilibrios de acróbata en el aire; y como prodigio de juegosmalabares, en una punta de caliza se mantenía, cual imantada, una bola grande debronce dorado, y encima otra más pequenya, y sobre ésta una cruz de hierro que acababaen pararrayos.'
mensaje_codificado, m2c = EncodeHuffman(mensaje)
mensaje_recuperado      = DecodeHuffman(mensaje_codificado,m2c)
print(mensaje_recuperado)

ratio_compresion        = 8 * len(mensaje) / len(mensaje_codificado)
print(ratio_compresion)

'''
Si tenemos en cuenta la memoria necesaria para almacenar el diccionario, 
¿cuál es la ratio de compresión?
'''
# Se puede guardar cada simbolo en orden alfabético de la siguiente manera: 4bits para indicar cuántos bits a continuación
# corresponden la codificación de dicha letra (asumimos que no serán más de 16bits) y luego los bits correspondientes a la codificación

dict_size = 0
for k, v in m2c.items():
    dict_size += len(v) + 4

print("Ratio con diccionario: " + str(8*len(mensaje)/(len(mensaje_codificado) + dict_size)))



print("== END 2.1 ==")