#imports all the libraries needed 
import numpy as np
import h5py
import serial
   
#defines our databank
#defines our serial communication with the teensy, to get the values and use them to input into the data bank we will send to the AI
dataBank = h5py.File('dataBankRigel.h5','a')
ser = serial.Serial('COM13',timeout=1)
ser.reset_input_buffer()

#creates a loop
while(1):

    #resets the input buffer
    ser.reset_input_buffer()
    #pass while waiting for data
    while(ser.in_waiting == 0):
        pass
    #definin our list as a vector
    list = []
    #for i from 1 to 30(the number of data that we have, 6 per MPU)
    for i in range (30):
    #puts the data in our list and data file and prints it in the monitor
        list.append(ser.readline().decode("utf-8").replace('/n', ""))
    data = np.ones((1,30, 300))
    for i in range (30):
        messy = np.fromstring(list[i], sep = " ")
        data[0][i] = messy
    print (data)

    #States that the data "x" in the bank will be defined as d1 = read, d2 = time and d3 = channel
    if not('x' in dataBank):
        xData = dataBank.create_dataset("x",(0, 300, 30),dtype="i4",maxshape=(None, None, 30))
        xData.attrs['d1'] = 'read'
        xData.attrs['d2'] = 'time'
        xData.attrs['d3'] = 'channel'

    #defines now the "y" data
        yData = dataBank.create_dataset("y", (0,), dtype="i", maxshape=(None,))
        yData.attrs['d1'] = 'read'
   
   #ask the user to write what gesture was just imported from the MPU6050 read
    gesture = input("write gesture to be inserted")
    #after you input the number of the gesture, the data will be sent to the bank as a graph
    #One important thing to notice is: to verify how constant your data is, you have to open your databank
    #and go to the "x" section, there, you will find parameters D0 and D2, D0 will be the input number, thus, the first gesture
    #that you input will be saved as 0 in D0, the second will be 1 in D0, and so on, for D2, there section will always range from
    #0 to 29, where each number is a different value given by the MPU, in our case, the order will be Ax, Ay, Az, Gx, Gy, Gz
    #and the order of the fingers will be pinky, ring, middle, index, thumb
    if(gesture.isnumeric):
        gesture = int(gesture)
        if(gesture>=0 and gesture<10):
            dataBank['x'].resize(dataBank['x'].shape[0] + 1, axis = 0)
            dataBank['x'][-1] = np.transpose(data,(0,2,1))


            dataBank['y'].resize(dataBank['y'].shape[0] + 1, axis = 0)
            dataBank['y'][-1] = gesture

#closes the files that were open previously
ser.close()
dataBank.close()