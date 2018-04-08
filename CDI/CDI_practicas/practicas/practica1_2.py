# -*- coding: utf-8 -*-
from collections import Counter


print("== START 1.2 ==")

'''
Dada la lista L de longitudes de las palabras de un código 
q-ario, decidir si pueden definir un código.

'''

def  kraft1(L, q=2):
    sum = 0
    for l in L:
        sum += pow(q, -l)
    return sum <= 1


'''
Dada la lista L de longitudes de las palabras de un  
código q-ario, calcular el máximo número de palabras 
de longitud Ln, que se pueden añadir y seguir siendo 
un código.
'''
def kraft3(L, Ln, q=2):
    is_code = True
    L_cpy = L.copy()
    amount = 0
    while is_code:
        L_cpy.append(Ln)
        is_code = kraft1(L_cpy, q)
        if is_code:
            amount += 1
    return amount


'''
    Dada la lista L de longitudes de las palabras de un código
    q-ario, calcular el máximo número de palabras de longitud
    máxima, max(L), que se pueden añadir y seguir siendo un código.
    
    '''
def kraft2(L, q=2):
    return kraft3(L, max(L), q)


'''
Dada la lista L de longitudes de las palabras de un  
código q-ario, hallar un código prefijo con palabras 
con dichas longitudes
'''
def get_symbol_from_id(id):
    return chr(48 + id)


def code(L,q=2):
    L_cpy = L.copy()
    L_cpy.sort()
    last_used_symbol_id = [0] * max(L_cpy)
    inc_id = 0  # which symbol of last_used_symbol has to be incremented
    code = []
    for l in L_cpy:
        if last_used_symbol_id[inc_id] == q-1:
            inc_id += 1
        str_code = ''
        for i in range(l):
            str_code += get_symbol_from_id(last_used_symbol_id[i])
        last_used_symbol_id[inc_id] += 1
        code.append(str_code)
    return code

'''
Ejemplo
'''

L=[1,3,5,5,10,3,5,7,8,9,9,2,2,2]
print(sorted(L),' codigo final:', code(L,3))
print(kraft1(L,3))


print("== END 1.2 ==")