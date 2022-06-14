# -*- coding: utf-8 -*-
"""
Created on Sat Jun 11 21:19:13 2022

@author: bruno
"""

import statistics as st
import numpy as np

valores_med = [[0.1039,0.1039,0.1039,0.1038,0.1038],
              [0.102,0.102,0.102,0.102,0.102],
              [1.039,1.039,1.038,1.038,1.037],
              [1.03,1.03,1.03,1.03,1.02],
              [9.08,9.08,9.08,9.08,9.07,9.07,9.07,9.06,9.06,9.06],
              [9.09,9.09,9.09,9.09,9.09,9.09,9.09,9.09,9.09,9.09]]
              
vv = [0.10352,0.102474,1.036676,1.029638,9.0543,9.02333]

delta_vv = (0.05/100 + 5/(0.1*1000))*0.1

for i in range(len(valores_med)):
    
    prom_vmed = st.mean(valores_med[i])#promedio
    
    ecal = prom_vmed - vv[i]

    #±0.5% ± 2 digits
    delta_vi = (0.5/100 + 2/(prom_vmed*1000))*prom_vmed

    svi= st.stdev(valores_med[i]) #desvio estandar de los valores medidos

    ua_vi = svi/np.sqrt(len(valores_med[i])) #Error tipo a de los valores medidos
    ub_vi = delta_vi/np.sqrt(3)

    uc_vi = np.sqrt(ua_vi**2 + ub_vi**2)
    uc_vv = delta_vv/np.sqrt(3)

    uc_ecal = np.sqrt(uc_vi**2 + uc_vv**2)

    U_ecal = uc_ecal*2 #95%

    if (ecal + U_ecal) <= delta_vi:
        print("Verifica la calibracion")
    else:
        print("No verifica la calibracion")
