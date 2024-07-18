import sensor, image, time
import ustruct
import utime
from pyb import UART

# Reset the image sensor to ensure a clean starting state
sensor.reset()

#Increased contrast for clearer features
sensor.set_contrast(1)
#Adjusted gain ceiling for better low-light performance
sensor.set_gainceiling(32)
sensor.set_auto_gain(False, gain_db=20)
#Vertical flip for image mirroring if needed
sensor.set_vflip(True)
#QVGA resolution and grayscale format for efficiency and face detection suitability
sensor.set_framesize(sensor.QVGA)
sensor.set_pixformat(sensor.GRAYSCALE)
#Windowing to restrict processing area (optional, adjust coordinates as needed)
sensor.set_windowing((160, 240))

#Load the Haar Cascade classifier for face detection
face_cascade = image.HaarCascade("frontalface", stages=100)
print(face_cascade)  #Print information about the cascade

#Initialize a clock for frame rate calculation
clock = time.clock()

#Create a UART object for communication
uart = UART(3, 38400, timeout_char=1000)  #Adjust baud rate and timeout as needed

#Starts a loop
while True:
    #Start the clock tick for frame rate measurement
    clock.tick()

    #Capture a snapshot from the image sensor
    img = sensor.snapshot()

    #Find faces in the captured image using the Haar Cascade classifier
    #Threshold: Higher threshold reduces false positives but might miss faces
    #Scale factor: Lower factor detects smaller faces at the cost of speed
    objects = img.find_features(face_cascade, threshold=0.97, scale_factor=1.25)

    #If no faces are detected, send a default center_y value
    if objects == []:
        center_y = 165
        uart.write(ustruct.pack("<h", center_y))  #Pack the value for transmission
        print(center_y)  #Print for testing

    #Draw rectangles around detected faces and transmit center coordinates
    for r in objects:
        #Draw a rectangle on the image to visualize the detection
        img.draw_rectangle(r)

        #Extract the rectangle coordinates (x, y, width, height)
        x, y, w, h = r

        #Filter out small objects (adjust size thresholds as needed)
        if w > 20 and h > 20:
            center_x = x + w // 2  #Calculate center coordinates
            center_y = y + h // 2

            #Send the center_y value serially using structured packing
            uart.write(ustruct.pack("<h", center_y))  #Pack as a 16-bit signed integer

            #Print for debugging or visualization
            print(center_y)

            #Introduce a short delay to avoid overwhelming the communication channel
            utime.sleep(0.5)  #Adjust delay as needed
        else:
            #If the object is too small, send a default center_y
            print(center_y)
            center_y = 165
            uart.write(ustruct.pack("<h", center_y))
