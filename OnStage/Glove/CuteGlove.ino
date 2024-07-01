//This is the code that we developed to read the values from the sensors and to make the RandomForest to answer what is the
//gesture that is being done, there is also a section for bluetooth communication where we send what was the gesture to the
//body teensy


//Includes the libraries we are usgin
#include <MPU6050.h>
#include <Wire.h>
#include <Metro.h>
#include <SPI.h>
#include <SD.h>
//Includes the RandomForest file
#include "RandomForestRegressor.h"

//Initiate timer instances that work just like Millis(), but are way better to use
Metro serialMetro = Metro(10);
Metro serialMetro2 = Metro(1000);

//Declares the MPU's and tells where each one of them is atached on Teensy 4.1
MPU6050 mpuFinger1(0x68, &Wire);
MPU6050 mpuFinger2(0x69, &Wire);
MPU6050 mpuFinger3(0x68, &Wire1);
MPU6050 mpuFinger4(0x68, &Wire2);
MPU6050 mpuFinger5(0x69, &Wire2);

//Creates a vector to keep data from all the sensors
MPU6050* vectorSensor[5] = { &mpuFinger1, &mpuFinger2, &mpuFinger3, &mpuFinger4, &mpuFinger5 };

//Generates a C code from python sklearn classifiers
Eloquent::ML::Port::RandomForest RF;




//Initializes variables and vectors that will further be used to calculate the Euler Angles we are getting and bools to control the process
float dt = 0.010;
float Alpha = 0.05;
float ThetaMinus1[5];
float Theta[5];
float Phi[5];
float PhiMinus1[5];
bool Metro2 = false;
int recordingCount = 0;
bool lastDown = false;
bool recordingMovement = false;
bool waiting = false;

//Creates the vector that will keep all the values
int valuesGesture[30][300];

//Creates vectors to save the Ax, Ay, Az, Gx, Gy, Gz and calibrate them for each sensor
int values[6][5];
int valuesCalib[6][5];


void setup() {

  //Initializes the vibracall motor
  pinMode(23, OUTPUT);
  digitalWrite(23, LOW);

  //Initializes the SDA/SCL communication that the MPus utilize
  Wire.begin();
  Wire1.begin();
  Wire2.begin();

  //Initializes the MPUs for the 5 fingers
  for (int i = 0; i < 5; i++) {
    vectorSensor[i]->initialize();
  }

  //Sets as 100000 the clock for the SDA/SCL communication
  Wire.setClock(100000);
  Wire1.setClock(100000);
  Wire2.setClock(100000);

  //Calls the void to calibrate the sensors
  Calibrate();

  //Starts serial communication with the computer(just for testing) and for bluetooth connection with the body
  Serial.begin(115200);
  Serial1.begin(38400);

  //Resets timer for serialMetro
  serialMetro.reset();

  //Setting as high a pin from the bluetooth module(this action is necessary in order to make it able to communicate with other devices)
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
}


void loop() {
  //Whenever the time determined by serialMetro passes, this action will repeat
  if (serialMetro.check() == 1) {
    //long teste = micros();
    for (int i = 0; i < 5; i++) {
      //declaring, storing and adjusting all the 3 gyro and 3 acc values for the 5 fingers
      int16_t aX, aY, aZ, gX, gY, gZ;
      vectorSensor[i]->getMotion6(&aX, &aY, &aZ, &gX, &gY, &gZ);
      values[0][i] = aX;
      values[1][i] = aY;
      values[2][i] = aZ;
      values[3][i] = gX - (int16_t)(valuesCalib[3][i] / 100);
      values[4][i] = gY - (int16_t)(valuesCalib[4][i] / 100);
      values[5][i] = gZ - (int16_t)(valuesCalib[5][i] / 100);
      /*transforms the raw values from the MPUs into the desired Euler Angles and also checks if any sensor is disconected
      because this way its values would be given as "nan", and this would mess with ThetaMinus1 and PhiMinus1, which could damage the collected data */ 
      Theta[i] = Alpha * (ThetaMinus1[i] + values[3][i] * dt) + (1 - Alpha) * ((180 / 3.1415) * atan(aX / (sqrt(aY * aY + aZ * aZ))));
      if (!std::isnan(Theta[i])) ThetaMinus1[i] = Theta[i];
      Phi[i] = Alpha * (PhiMinus1[i] + values[4][i] * dt) + (1 - Alpha) * ((180 / 3.1415) * atan(aY / (sqrt(aX * aX + aZ * aZ))));
      if (!std::isnan(Phi[i])) PhiMinus1[i] = Phi[i];
    }
    //checks if a move should be recorded
    if (recordingMovement == true) {
      //gets all the 6 values from the 5 sensors and adds 1 to our recordingCount variable
      for (int i = 0; i < 5; i++) {
        for (int o = 0; o < 6; o++) {
          valuesGesture[i * 6 + o][recordingCount] = values[o][i];
        }
      }
      recordingCount++;
    }

    //Remove the comment below if you want to verify the values for the MPUs
    /*for (int i = 0; i < 5; i++) {
      Serial.print("phi");
      Serial.print(i);
      Serial.print(":");
      Serial.println(Phi[i]);
    }*/
    /*for (int i = 0; i < 5; i++) {
      Serial.print("theta");
      Serial.print(i);
      Serial.print(":");
      Serial.println(Theta[i]);
    }*/
  }

  //When the control variable "recordingCount" reaches 300, it means that we got all the values that we wanted, thus, it can stop recording
  if (recordingCount == 300) {
    //the movement must not be recorded anymore
    recordingMovement = false;
    //Metro2 does not need to be true anymore
    Metro2 = false;
    //stops the vibracall
    digitalWrite(23, LOW);
    //resets the count
    recordingCount = 0;
    //get and send the gesture recorded
    Send();
    getGesture();
  }

  //This is to communicate via bluetooth with other devices
  /*if (Serial1.available()) {
    Serial.write(Serial1.read());
  }*/
  
  //To control better the movements we are recording, the glove will only do so if all the 5 fingers are pointed down
  if (Phi[0] < -45 && Phi[1] < -45 && Phi[2] < -45 && Phi[3] < -45 && Phi[4] < -45) {
    //lastDown is true because all the fingers just got down
    lastDown = true;
    //if the glove is not recording nor waiting for a movement
    if (recordingMovement == false && waiting == false) {
      //the vibracall starts to warn the user that something is being recorder
      digitalWrite(23, HIGH);
      //the glove is now waiting for a gesture
      waiting = true;
      //1 second delay to let the user process that it shall now perform a movement
      delay(1000);
    }
    //checks if it must record a movement
    if (recordingMovement == true) {
      //checks for the Metro2 timer
      if (serialMetro2.check() == 1) {
        //sets Metro2 as true to use in the next if
        Metro2 = true;
      }
      //if the time determined by Metro2 passes
      if (Metro2 == true) {
        //The vibracall is turned off
        digitalWrite(23, LOW);
        //The movement is not recording anymore  
        recordingMovement = false;
        //The count resets
        recordingCount = 0;
        //The data is sent and the gesture is determined
        Send();
        getGesture();
        //Metro2 is now false
        Metro2 = false;
      }
    }
  } 
  //If the metro is not true but waiting is
  else {
    if (waiting == true) {
      //a movement should be recorded
      recordingMovement = true;
      //the Metros are reseted
      serialMetro2.reset();
      serialMetro.reset();
      //The vibracall is again turned on
      digitalWrite(23, HIGH);
      //The glove is not waiting for a recording anymore and the count is set to 0
      waiting = false;
      recordingCount = 0;
    }
  }
}

//void to get the gesture and send it to the python program
void getGesture() {
  //creates the vector to send the values to python and initializes a count variable
  float sendToPython[465];
  int count = 0;
  //Gets all the values and the correlations between them and put them into the sendToPython vector using the count variable
  for (int i = 0; i < 30; i++) {
    for (int o = i + 1; o < 30; o++) {
      sendToPython[count] = correlation(valuesGesture[i], valuesGesture[o], 300);
      count++;
    }
  }
  //sends al the wanted values to python, so that we could register and train our random forest
  for (int p = 0; p < 30; p++) {
    sendToPython[p + 435] = mean(valuesGesture[p], 300);
  }
  //gets the prediction from the random forest and prints it
  Serial.println(RF.predict(sendToPython));
  //concat and send the data via bluetooth to the body
  String data = "AT+DATA1";
  data.concat(RF.predict(sendToPython));
  data.concat("\r\n");
  Serial1.println(data);
}

//Prints the values from the vectors(this is just used for geting data and performing comparisons)
void Send() {
  for (int i = 0; i < 5; i++) {
    for (int o = 0; o < 6; o++) {
      for (int p = 0; p < 300; p++) {
        Serial.print(valuesGesture[i * 6 + o][p]);
        Serial.print(" ");
        delayMicroseconds(100);
      }
      Serial.println("");
      delay(5);
    }
  }
}

//Calibration void to make the data more stable, consistent and coherent
void Calibrate() {
  for (int i = 0; i < 100; i++) {
    for (int o = 0; o < 5; o++) {
      int16_t aX, aY, aZ, gX, gY, gZ;
      vectorSensor[o]->getMotion6(&aX, &aY, &aZ, &gX, &gY, &gZ);
      valuesCalib[0][o] = aX + valuesCalib[0][o];
      valuesCalib[1][o] = aY + valuesCalib[1][o];
      valuesCalib[2][o] = aZ + valuesCalib[2][o];
      valuesCalib[3][o] = gX + valuesCalib[3][o];
      valuesCalib[4][o] = gY + valuesCalib[4][o];
      valuesCalib[5][o] = gZ + valuesCalib[5][o];
    }
    delay(5);
  }
}

//cleans the vector
void cleanVectors() {
  memset(valuesGesture, 0, sizeof(valuesGesture));
}





//does the mean, that was used above, for our values
float mean(int X[], int n) {
  float sumMean = 0;
  for (int i = 0; i < n; i++) {
    sumMean = sumMean + X[i];
  }
  sumMean = sumMean / n;
  return sumMean;
}

//states the r correlation between our colected data
float correlation(int X[], int Y[], int n) {
  //creates all the needed variables
  float r = 0;
  float sumX = 0;
  float sumY = 0;
  float sumXsqr = 0;
  float sumYsqr = 0;
  float sumXY = 0;
  for (int i = 0; i < n; i++) {
    //gets the sum from the operations between the values
    sumX = sumX + X[i];
    sumY = sumY + Y[i];
    sumXY = sumXY + (X[i] * Y[i]);
    sumXsqr = sumXsqr + (X[i]) * (X[i]);
    sumYsqr = sumYsqr + (Y[i]) * (Y[i]);
  }
  //calculates the relation r between the data, this will help us to determine what gesture is being performed
  r = ((n * sumXY) - (sumX * sumY)) / (sqrt(((n * sumXsqr) - (sumX * sumX)) * ((n * sumYsqr) - (sumY * sumY))));
  //returns the relation r
  return r;
}
