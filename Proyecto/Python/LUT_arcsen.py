# -*- coding: utf-8 -*-
"""
Created on Sat Sep 24 15:33:11 2022

@author: bruno
"""
import numpy as np
bits = 16
val_max = (2**bits)/2/2 #2^16 --> [-2g,2g], nosotros usamos [-1g,1g]
val_min = -(2**bits)/2/2

cant_ang = 180
val = [0 for x in range(cant_ang*4)]#creo un array de 180*4 elementos
x = np.linspace(0,cant_ang,cant_ang*4)

k = 0.01745329

for i in range(cant_ang*4):
    val[i] = val_max*(np.sin(x[i]*k))
    
with open("LUT_sin.txt", "w") as ext_file:
    add_text = 'const uint16_t LUTsin[360] = \n{'
    print(add_text, file=ext_file)
    
    for i in range(144):
        i = i*5
        print(int(val[i]),',',int(val[i+1]),',',int(val[i+2]),',',int(val[i+3]),',',int(val[i+4]),',', file=ext_file)
    ext_file.close()    
