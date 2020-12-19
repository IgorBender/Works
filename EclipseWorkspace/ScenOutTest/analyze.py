#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""

Created on Sat Mar  9 14:40:55 2019

@author: igor
"""

import plotly
import plotly.graph_objs as go
from plotly import tools
import numpy as np
import sys

if len(sys.argv) < 2:
	print('Usage : ', sys.argv[0], ' file')
	exit(1)

d = np.loadtxt(sys.argv[1])
numrows = len(d)
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
          "mode" : "markers", "name" : "Trajectory", "type" : "scatter3d" }
trace2 = {"x": frame, "y" : az, "marker" : { "color" : "blue", "size" : 2 },
          "mode" : "markers", "name" : "Azimuth", "type" : "scatter" }
trace3 = {"x": frame, "y" : el, "marker" : { "color" : "blue", "size" : 2 },
          "mode" : "markers", "name" : "Elevation", "type" : "scatter" }
trace4 = {"x": az, "y" : az, "marker" : { "color" : "blue", "size" : 2 },
          "mode" : "markers", "name" : "Trajectory", "type" : "scatter" }

fig = tools.make_subplots(rows=2, cols=2, 
    specs=[[{'is_3d': False}, {'is_3d': False}],
           [{'is_3d': False}, {'is_3d': True}]])
#    subplot_titles=('Azimuth', 'Elevation',
#                                                          'Trajectory', '3D'),
fig.append_trace(trace2, 1, 1)
fig.append_trace(trace3, 1, 2)
fig.append_trace(trace4, 2, 1)
fig.append_trace(trace1, 2, 2)

#layout = go.Layout(
##	title='Trajectory',
#
#	scene = dict(
#	xaxis=dict(
#	   title='Frame'
#    ),
#	yaxis=dict(
#	   title='Azimuth'
#    ),
#	zaxis=dict(
#	   title='Elevation'
#    )
#    ),
##    xaxis=dict(
##        title='Frame'
##        ),
##    yaxis=dict(
##        title='Azimuth'
##        ),
##    xaxis2=dict(
##        title='Frame'
##        ),
##    yaxis2=dict(
##        title='Elevation',
##        ),
##    xaxis3=dict(
##        title='Azimuth'
##        ),
##    yaxis3=dict(
##        title='Elevation',
##        ),
#    margin=dict(
#        l=0,
#        r=0,
#        b=0,
#        t=0
#    )
#)

#fig = go.Figure(data=data, layout=layout)
plotly.offline.plot(fig)
