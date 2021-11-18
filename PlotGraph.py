# -*- coding: utf-8 -*-
"""Untitled0.ipynb

Automatically generated by Colaboratory.

Original file is located at
    https://colab.research.google.com/drive/1X1DySH1Q1bGWX3A_x52spissBV7b_uD5
"""

# import pip
# !pip install pygrok
from pygrok import Grok
from matplotlib import pyplot as plt
import numpy as np


listAll = []
with open('/content/throughput.txt', 'rb') as f:
 for line in f:
        listAll.append(line)

del listAll[:6]

# parse String
pattern = "flow%{NUMBER:num}  %{NUMBER:sec}s: 	%{NUMBER:size} Mbit/s" 
grok = Grok(pattern)
dicts = []
for val in listAll:
  str = val.decode('utf-8')
  dicts.append(grok.match(str))

# set values  
flow0 = []
flow1 = []
flow2 = []
flow3 = []

time = []

for dict in dicts:
  for key,val in dict.items():
    if key == 'num':
       if val == '0':
         flow0.append(float(dict['size']))
         time.append(float(dict['sec']))
       if val == '1':
         flow1.append(float(dict['size']))
       if val == '2':
         flow2.append(float(dict['size']))
       if val == '3':      
         flow3.append(float(dict['size']))

 # plot the graph        
plt.plot(time,flow0,'b',label='flow0')
plt.plot(time,flow1,'r',label='flow1')
plt.plot(time,flow2,'g',label='flow2')
plt.plot(time,flow3,'y',label='flow3')
plt.xlabel('time axis')
plt.ylabel('data axis')  
plt.title('Data flow')
plt.legend(loc='upper right')
plt.show()