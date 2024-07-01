import numpy as np 
import h5py
import serial 

dataBank = h5py.File('dataBank.h5','a')
ser = serial.Serial('COM18',timeout=1)
ser.reset_input_buffer()

while(1): 
    ser.reset_input_buffer()
    while(ser.in_waiting == 0):
        pass
    lista = []
    for i in range (30):
        lista.append(ser.readline().decode("utf-8").replace('/n', ""))
    dados = np.ones((1,30, 300))
    for i in range (30):
        bagunssa = np.fromstring(lista[i], sep = " ")
        dados[0][i] = bagunssa
    print (dados)
    print(ser.readline().decode("utf-8").replace('/n', ""))

    if not('x' in dataBank):
        xData = dataBank.create_dataset("x",(0, 600, 30),dtype="i4",maxshape=(None, None, 30))
        xData.attrs['d1'] = 'read'
        xData.attrs['d2'] = 'time'
        xData.attrs['d3'] = 'channel'

        yData = dataBank.create_dataset("y", (0,), dtype="i", maxshape=(None,))
        yData.attrs['d1'] = 'read'
    
    gesto = input("escreva o gesto a ser inserido")
    if(gesto.isnumeric()):
        gesto = int(gesto)
        if(gesto>=0 and gesto<10):
            dataBank['x'].resize(dataBank['x'].shape[0] + 1, axis = 0)
            dataBank['x'][-1] = np.transpose(dados,(0,2,1))

            dataBank['y'].resize(dataBank['y'].shape[0] + 1, axis = 0)
            dataBank['y'][-1] = gesto

ser.close()
dataBank.close()