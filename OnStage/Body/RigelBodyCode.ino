//engrenagem motor 1:2
//gear-motor reduction 1:2
//gear-encoder reduction 1:3
//1 wheel rotation = 35pi/2 cm
//1 wheel rotation = 2400 pulses on encoder
//distance between wheels' centers = 321.5mm

//starting the libraries
#include <SD.h>
#include <AccelStepper.h>
#include <Metro.h>
#include <AccelStepper.h>
#include <Servo.h>
#include <Adafruit_BNO055.h>

//defining arm states
#define ARM_IDLE 0
#define ARM_MOVING_FORWARD 1
#define ARM_MOVING_BACKWARD 2

//defining our switches and the LDR
#define botForearmL 41
#define botArmL 40
#define botForearmR 39
#define botArmR 38
#define ldrPin 16
#define botDoorL 15
#define botDoorR 14

//creates a variable to control the Serial print, this variable is just used when we are testing
#define printOrNot 1

//defines our servo pins
#define servoHandLPin 23
#define servoDoorLPin 22
#define servoHandRPin 21
#define servoDoorRPin 20

//**excluir aqui?
#define resetBNO 13

//creates a variable to update the arm state
int armState = ARM_IDLE;

//Declares our step motors
AccelStepper ForeArmL(1, 8, 9);  //parameters, 1-related to the driver, they are fixed for each one, 2-pulse pin, 3- direction pin
AccelStepper ArmL(1, 11, 12);
AccelStepper ForeArmR(1, 31, 32);
AccelStepper ArmR(1, 28, 29);
AccelStepper Head(1, 25, 26);
AccelStepper stepperL(1, 5, 6);
AccelStepper stepperR(1, 2, 3);

//Defines our SD file, this will be used to read the trajectory
File bin;

//creates a variable to control time
int timePass = 0;


int i = 0;


//creates two strings to store the bluetooth read and serial read. leitura = read in portuguese, we are using it instead of read because this word might conflict with already existing functions on the IDE
String leituraBT = "";
String leituraSerial = "";

//setting variables to keep the values read by the encoder
volatile long stepLeft = 0;
volatile long stepRight = 0;

//Creates our servos instances
Servo handL;
Servo handR;
Servo doorL;
Servo doorR;

//create some variables that will be used on the trajectory reading system
float kp = 0;
float kd = 0;
float tau = 0.002;
float lastSpeedL = 0;
float lastSpeedR = 0;




//variables for distance, velocity(dT) and acceleration(ddT) respectively
//L is for left, R is for right
float thetaL;
float thetaLDOt;
float thetaLDDOt;
float thetaR;
float thetaRDOt;
float thetaRDDOt;

//Initializes two variables that will help us to connect with the bluetooth of the body and the glove in the correct order
int gloveNumber = 1;
int headNumber = 2;

//metro is a function on teensy similar to miles, however, metro is way easier to use
Metro dt = Metro(2);

//Declares our BNO gyroscope
Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28, &Wire);

void setup() {
  //Declaring the pins from the encoders as input_pullups because this functions ties the pin to 5V with an internal 10K resistor
  pinMode(0, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  pinMode(36, INPUT_PULLUP);
  pinMode(37, INPUT_PULLUP);

  //?
  attachInterrupt(digitalPinToInterrupt(0), white1, RISING);
  attachInterrupt(digitalPinToInterrupt(17), green1, RISING);
  attachInterrupt(digitalPinToInterrupt(36), white2, RISING);
  attachInterrupt(digitalPinToInterrupt(37), green2, RISING);








  //Begins the relationship of the teensy with the SD
  SD.begin(BUILTIN_SDCARD);

  //Initializes the communication with Serial port 8(bluetooth)
  Serial8.begin(38400);

  //setting max speed, acceleration and minimum pulse widht for each one of our step motors
  stepperL.setMaxSpeed(1500);
  stepperR.setMaxSpeed(1500);
  stepperL.setAcceleration(1000);
  stepperR.setAcceleration(1000);
  stepperL.setMinPulseWidth(10);
  stepperR.setMinPulseWidth(10);
  ArmL.setMaxSpeed(1000);
  ArmL.setAcceleration(1000);
  ArmL.setMinPulseWidth(10);
  ArmR.setMaxSpeed(1000);
  ArmR.setAcceleration(1000);
  ArmR.setMinPulseWidth(10);
  ForeArmL.setMaxSpeed(1000);
  ForeArmL.setAcceleration(200);
  ForeArmL.setMinPulseWidth(10);
  ForeArmR.setMaxSpeed(1000);
  ForeArmR.setAcceleration(200);
  ForeArmR.setMinPulseWidth(10);
  Head.setMaxSpeed(2000);
  Head.setAcceleration(600);
  Head.setMinPulseWidth(10);

  //Stating what are the enable pins for each one of our motors
  //inverting needed pins on the motors to make things more logical
  stepperR.setEnablePin(1);
  stepperR.setPinsInverted(true, false, true);  //inverts the pins from stepperR(right wheel stepper-motor), we do this just to visually facilitate declaring velocity and distance on the motors, as this way the values will have the same signal for both motors
  stepperL.setEnablePin(4);
  stepperL.setPinsInverted(false, false, true);
  ForeArmL.setEnablePin(7);
  ForeArmL.setPinsInverted(false, false, true);
  ArmL.setEnablePin(10);
  ArmL.setPinsInverted(true, false, true);
  Head.setEnablePin(24);
  Head.setPinsInverted(false, false, true);
  ArmR.setEnablePin(27);
  ArmR.setPinsInverted(false, false, true);  //direct
  ForeArmR.setEnablePin(30);
  ForeArmR.setPinsInverted(true, false, true);

  //Disabling our motors while they are not being used, this helps a lot on saving energy :)
  stepperR.disableOutputs();
  stepperL.disableOutputs();
  ForeArmL.disableOutputs();
  Head.disableOutputs();
  ForeArmR.disableOutputs();

  //?
  ArmL.enableOutputs();
  ArmR.enableOutputs();


  //?
  pinMode(resetBNO, OUTPUT);
  digitalWrite(resetBNO, LOW);
  delay(100);
  digitalWrite(resetBNO, HIGH);

  //?
  pinMode(33, OUTPUT);
  digitalWrite(33, LOW);
  //?
  analogReadAveraging(16);

  Serial.begin(115200);  //starts serial coms at a baud rate of 115200
  dt.reset();            //resets the dt(the Metro variable declared above)
  //Begins the BNO readings
  bno.begin();
  //delay(2000);

  //resets stepLeft and stepRight to 0
  stepLeft = 0;
  stepRight = 0;

  /*while (1) {
    int leituraBotForeArmL = analogRead(botForearmL);
    if (printOrNot) {
      Serial.println(leituraBotForeArmL);
    }
    int leituraBotForeArmR = analogRead(botForearmR);
    if (printOrNot) {
      Serial.print("leitura BotForeArmR");
      Serial.println(leituraBotForeArmR);
    }
  }*/

  //runs our startingSetup void, this void helps us to set up for the start of the presentation
  startingSetup();
  delay(2000);
  //send a command for the eyes to blink, this helps us to verify if the bluetooths were properly connected
  writeBluetoothOthers("BLINK", false);


}

void loop() {
  //Sets the needed voids for our presentation
  Begining();
  WaitingForGesture(1);
  WaitingForGesture(2);
  WaitingForGesture(3);
  changeToMacarena1();
  WaitingForGesture(4);
  madBecauseNoMoreMacarena();
  noMoreHead();
  WaitingForGesture(5);
  end();
  while (1)
    ;
  //This void will be used for our technical interview, it contains a easier way to demonstrate everything we can do but separetely
  //interview();
}
