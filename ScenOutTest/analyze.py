#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Mar  9 14:40:55 2019

@author: igor
"""

from mpl_toolkits import mplot3d
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import interactive
import sys

interactive(True)

if 2 != len(sys.argv):
    print("Usage : {} file".format(sys.argv[0]))
    exit()

data = np.loadtxt(sys.argv[1])
numrows = len(data)
frame = []
az = []
el = []
for i in range(numrows):
    frame.append(data[i][0])
    if data[i][1] > 0:
        az.append(data[i][3])
        el.append(data[i][4])
    else:
        az.append(np.nan)
        el.append(np.nan)

plt.subplot(2, 2, 1)
plt.plot(frame, az, '.', ms=2)
plt.title('Azimuth')
plt.grid(True)
plt.grid(True, 'minor')
plt.minorticks_on()
plt.xlabel('Frame')
plt.ylabel('Az')
plt.subplot(2, 2, 2)
plt.plot(frame, el, '.', ms=2)
plt.title('Elevation')
plt.grid(True)
plt.grid(True, 'minor')
plt.minorticks_on()
plt.xlabel('Frame')
plt.ylabel('El')
plt.subplot(2, 2, 3)
plt.plot(az, el, '.', ms=2)
plt.title('Trajectory')
plt.grid(True)
plt.grid(True, 'minor')
plt.minorticks_on()
plt.xlabel('Az')
plt.ylabel('El')

ax = plt.subplot(2, 2, 4, projection='3d')
plt.plot(frame, az, el, '.', ms=2)
plt.grid(True)
plt.grid(True, 'minor')
plt.minorticks_on()
plt.xlabel('Frame')
plt.ylabel('AZ')
ax.set_zlabel('El')
ax.view_init(30, 360 - 45)
plt.tight_layout()

plt.show()
input()
