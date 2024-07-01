import numpy as np
import h5py
import serial
   
#defines our databank
#defines our serial communication with the teensy, to get the values and use them to input into the data bank we will send to the AI
dataBank = h5py.File('dataBankRigel.h5','a')

dataBank['x'].resize(dataBank['x'].shape[0] - 1, axis = 0)
dataBank['y'].resize(dataBank['y'].shape[0] - 1, axis = 0)