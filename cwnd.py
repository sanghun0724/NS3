# -*- coding: utf-8 -*-
"""cwnd.ipynb

Automatically generated by Colaboratory.

Original file is located at
    https://colab.research.google.com/drive/19Ea7FV7Xbvap9Y1jv_46GFUSH43f63V7
"""

# import pip
# !pip install pygrok
from matplotlib import pyplot as plt
import numpy as np


listAll = []
with open('/content/cwnd.txt', 'rb') as f:
 for line in f:
   findIndex = line.find('Win='.encode())
   if findIndex != -1:
     listAll.append(float(line[findIndex+4:findIndex+9].decode('utf-8')))


print(len(listAll))
time = []
tmp = 0
for i in listAll:
  tmp = tmp + 0.0001
  time.append(tmp)


#  # plot the graph        
plt.plot(time,listAll,'ro')
# plt.axis(0,70000,0,700000)
plt.xlabel('time axis')
plt.ylabel('cwnd axis')  
plt.title('CWND flow')
plt.legend(loc='upper right')
plt.show()