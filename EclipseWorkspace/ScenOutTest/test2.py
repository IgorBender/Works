#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Mar  9 14:40:55 2019

@author: igor
"""

import plotly
import plotly.graph_objs as go
import numpy as np
import sys

if len(sys.argv) < 2:
	print('Usage : ', sys.argv[0], ' file')
	exit(1)

d = np.loadtxt(sys.argv[1])
numrows = len(d)
data = [[], [], []]
frame = []
az = []
el = []
for i in range(numrows):
        frame.append(d[i][0])
        active = d[i][1]
        if active == 1:
            az.append(d[i][3])
            el.append(d[i][4])
        else:
            az.append(np.NaN)
            el.append(np.NaN)
			
#trace1 = go.Mesh3d(x=frame, y = az, z= el, opacity=0.5, color='rgba(244,22,100,0.6)')
#layout = go.Layout(
#                    scene = dict(
#                    xaxis = dict(
#                        nticks=20, range = [0,numrows],),
#                    yaxis = dict(
#                        nticks=20, range = [-10,10],),
#                    zaxis = dict(
#                        nticks=20, range = [-10,10],),),
#                    width=700,
#                    margin=dict(
#                    r=20, l=10,
#                    b=10, t=10)
#                  )
#fig = go.Figure(data=[trace1], layout=layout)
#plotly.offline.plot(fig)

#trace1 = go.Scatter3d(
#    x=frame,
#    y=az,
#    z=el,
#    mode='dots',
#    marker=dict(
#        size=1,
#        color='blue'
#    )
#)
trace1 = {"x": frame, "y" : az, "z" : el, "marker" : { "color" : "blue", "size" : 1 },
          "mode" : "markers", "name" : "aaa", "type" : "scatter3d" }

layout = go.Layout(
	title='Trajectory',
	scene = dict(
	xaxis=dict(
	   title='Frame'
    ),
	yaxis=dict(
	   title='Azimuth'
    ),
	zaxis=dict(
	   title='Elevation'
    )
    ),
    margin=dict(
        l=0,
        r=0,
        b=0,
        t=0
    )
)

fig = go.Figure(data=[trace1], layout=layout)
plotly.offline.plot(fig)
