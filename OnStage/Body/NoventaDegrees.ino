//This void rotates 45 degrees with the gyroscope, the comments are the same as for the 90 degrees movement, if you have already read it, you can ignore the comments
void NoventaDegrees() {
  //?
  imu::Vector<3> eulerInit = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  //creates a reference value that will be the value it has started with
    //we have decided to do this because we were having trouble regarding our BNO, which was not able to reset and his starting points were never 0; thus, we are working only with deviations  
  double reference = eulerInit.x();
  //enables and sets differen parameters for the motors of the wheels, to rotate more smoothly
  stepperL.enableOutputs();
  stepperR.enableOutputs();
  stepperR.setMaxSpeed(500);
  stepperL.setMaxSpeed(500);
  stepperR.setAcceleration(250);
  stepperL.setAcceleration(250);
  //move to a really huge position so it just really stops when the gyro registers precisely the number that we want
  stepperL.moveTo(-90015); 
  stepperR.moveTo(90015);
  while (stepperL.distanceToGo() != 0 && stepperR.distanceToGo() != 0) {
    stepperL.run();
    stepperR.run();
    //?
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    //As we were having trouble with reseting the BNO, we are just assuming a new reference and getting the deviation in degrees, and as we are moving counterclockwise, the value for this deviation will be the replementary of what we want to rotate
    Serial.println((int)(euler.x() - reference + 360) % 360);
    //In this case(and also taking in consideradion the inertia effect on the movement), this code will make it stop whenver it rotates 90 degrees
    if (((int)(euler.x() - reference + 360) % 360) > 5 && ((int)(euler.x() - reference + 360) % 360) <= 273) {
      break;
    }
  }
  delay(1000);
  //The motors now stop and are disabled
  stepperL.setCurrentPosition(0);
  stepperR.setCurrentPosition(0);
  stepperL.disableOutputs();
  stepperR.disableOutputs();
}