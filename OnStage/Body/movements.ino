//This is the void to turn the wheel's step motors in degrees, as we are using the gyro now, this section of the code is not useful anymore
void turn(int degrees) {
  //defines a variable called "steps" that will convert the value inserted in degrees into steps by using the formula bellow
  int steps = (degrees / 360.0) * (2.0 * PI * 180) * (800.0 / (2 * PI * 87.5));
  //max speed and acceleration is reseted for the wheel's motors
  stepperL.setMaxSpeed(300);  
  stepperR.setMaxSpeed(300);
  stepperL.setAcceleration(100);
  stepperR.setAcceleration(100);
  //they are now enabled
  stepperL.enableOutputs();
  stepperR.enableOutputs();
  stepperL.setCurrentPosition(0);
  stepperR.setCurrentPosition(0);
  //They will turn on the angle you asked when calling this void
  stepperL.moveTo(steps);
  stepperR.moveTo(-steps);
  bool stop = false;
  while (!stop) {
    stepperL.run();
    stepperR.run();
    if (stepperL.distanceToGo() == 0 && stepperR.distanceToGo() == 0) stop = true;
  }
  //the steppers are now disabled and their parameters go back to what they were 
  stepperL.disableOutputs();
  stepperR.disableOutputs();
  stepperL.setMaxSpeed(1500); 
  stepperR.setMaxSpeed(1500);
  stepperL.setAcceleration(1000);
  stepperR.setAcceleration(1000);
}

//This void turns the head in degrees, which is very useful as we do not have a gyro on the head
void turnHead(int degrees) {
  //This time, a different formula is applied, but with the same purpose: transforming degrees into steps
  int steps = (degrees / 360.0) * (2600);
  Head.setCurrentPosition(0);
  //the head motor is enabled
  Head.enableOutputs();
  //The head turns on the desired angle
  Head.moveTo(steps);
  while (Head.distanceToGo() != 0) {
    Head.run();
  }
  Head.disableOutputs();
}