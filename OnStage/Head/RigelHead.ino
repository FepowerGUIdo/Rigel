/*
   Bluetooth pin: 17; SerialPin: 3
   Driver: Enable: 23, Pulse: 25, Direction: 27
   Aneis de Led: 45 e 47
   Servos: 49, 51 e 53
   Câmera: SerialPin: 1, SpecialPin: 16
   Pinguins: 2 e 3
*/
//Includes the libraries that we will be using throughout the code
#include <Servo.h>
#include <AccelStepper.h>
#include <Adafruit_NeoPixel.h>
#include <Metro.h>

//Such as it is for the body, here, we also declare a variable to use when printing things to test and check if everything is correct
#define printOrNot 1

//Defining the LED ring pins and declaring them
#define LED_PIN 45
#define LED_PIN2 47
#define LED_COUNT 16
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(LED_COUNT, LED_PIN2, NEO_GRB + NEO_KHZ800);

//Setting the RGB values to make the eyes look like they are a rainbow 
int Rrainbow = 86;
int Grainbow = 0;
int Brainbow = 172;

//Just like the arms system, the head also have a system controlled by state variables, those are being declared now
#define HeadIdle 0
#define Head_Moving_Upward 1
#define Head_Moving_Downward 2
int HeadState = HeadIdle;

//This string will store the command recieved via bluetooth from the body
String bluetoothCommand = "";

//Defining the pins for both of our switches, the enable for the vertical movement motor and the camera
#define PenUp 2
#define PenDown 3
#define ENHEAD 23
#define CamPin 16

//Declaring the motor from the head
AccelStepper HeadMotor(1, 25, 27); //parameters, 1-related to the driver, fixed for each motor, 2-pulse+ pin, 3-direction+ pin

//String created to store what is being read at the Serial monitor
String leituraSerial = "";

//Initializes all the 3 servos
Servo Mouth;
Servo EyebrowR;
Servo EyebrowL;

//initializes two metro instances
Metro resetBluetooth = Metro(12000);
Metro trackMetro = Metro(10000);

//Bool to check wether or not the bluetooth module is connected
bool isBlueConnected = false; 


void setup() {
  //defining both switches as INPUT_PULLUP because this will use internal resistors to pull up the state of the port
  pinMode(PenUp, INPUT_PULLUP);
  pinMode(PenDown, INPUT_PULLUP);
  pinMode(17, OUTPUT);
  digitalWrite(17, HIGH);

  //Initializing all of the Serial communications that will be used
  Serial.begin(9600);
  Serial1.begin(38400);
  Serial3.begin(38400);

  //sets a minimum pulse width for the step motor
  HeadMotor.setMinPulseWidth(10);

  
  HeadMotor.setPinsInverted(false, false, true); //inverts or not the pins of pulse, direction and enable respectively

  //declares the enable pin for the motor
  HeadMotor.setEnablePin(ENHEAD);

  //disables the motor, in order to save energy
  HeadMotor.disableOutputs();

  //sets the acceleration and max velocity
  HeadMotor.setAcceleration(200);
  HeadMotor.setMaxSpeed(1000);   

  HeadMotor.moveTo(-2000);

  //starts and clears the LED rings
  strip.begin();
  strip2.begin();
  strip.clear();
  strip2.clear();
  strip.show();
  strip2.show();
  
  //normalize to reset the head parameters
  normalize();

  //This commented part was used to test only the camera track, it might be useful when wanting to check if the camera is ok
  /*HeadMotor.enableOutputs();
  white2();
  while(1){
    track();
  }
  HeadMotor.disableOutputs();*/

  //Forces the connection of the JDY-24M with the body and blinks the eye after the connection succedes
  while(!isBlueConnected){
    readBluetooth();
    configBluetooth();
    if(bluetoothCommand == "BLINK"){
      blink();
    }
  }
  //just another verification to check if everything is really ok
  white();
  delay(1000);
  off();
}
void loop() {
  //normalize();
  //naughty();
  //happy();
  //sad();
  //mad();
  //yes();
  //track();

  //the string incoming from the bluetooth of the body will be stored
  readBluetooth();
  configBluetooth();
  

  //this string will them go through the comparisson below and each possible string will result in a different answer
  if (bluetoothCommand == "NAUGHTY") {
    naughty();
  }
  if (bluetoothCommand == "HAPPY") {
    happy();
  }
  if (bluetoothCommand == "SAD") {
    sad();
  }
  if (bluetoothCommand == "MAD") {
    mad();
  }
  if (bluetoothCommand == "YES") {
    yes();
  }
  if (bluetoothCommand == "START") {
    start();
  }
  if (bluetoothCommand == "ORANGE") {
    orange();
  }
  if (bluetoothCommand == "BLINK") {
    blink();
  }
  if (bluetoothCommand == "OFF") {
    off();
  }
  if (bluetoothCommand == "WHITE") {
    white();
  }
  if (bluetoothCommand == "TRACK") {
    //enable the motor
    HeadMotor.enableOutputs();
    //resets the metro instance
    trackMetro.reset();
    //tracks
    while(1){
      if(trackMetro.check()) break;
    track();
    }
    //disable motor
     HeadMotor.disableOutputs();
  }
  if (bluetoothCommand == "THANKS") {
    thanks();
  }
  if (bluetoothCommand == "NORMALIZE") {
    normalize();
  }
  //empties the string
  bluetoothCommand = "";
}
