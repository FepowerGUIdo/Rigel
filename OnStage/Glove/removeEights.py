import numpy as np 
import h5py
import serial 

dataBank2 = h5py.File('dataBank.h5','a')
dataBank = h5py.File('dataBankRigel.h5','a')

if not('x' in dataBank2):
    xData = dataBank2.create_dataset("x",(0, 600, 30),dtype="i4",maxshape=(None, None, 30))
    xData.attrs['d1'] = 'read'
    xData.attrs['d2'] = 'time'
    xData.attrs['d3'] = 'channel'

    yData = dataBank2.create_dataset("y", (0,), dtype="i", maxshape=(None,))
    yData.attrs['d1'] = 'read'

print(dataBank['x'][0])
for i in range(46):
        if(dataBank['y'][i]!=8):
            dataBank2['x'].resize(dataBank2['x'].shape[0] + 1, axis = 0)
            dataBank2['x'][-1] = dataBank['x'][i]

            dataBank2['y'].resize(dataBank2['y'].shape[0] + 1, axis = 0)
            dataBank2['y'][-1] = dataBank['y'][i]

dataBank.close()
dataBank2.close()