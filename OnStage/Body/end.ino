//This void ends the presentation with Rigel thanking the crows
void end() {
  //send "THANKS" to the head, meaning that the LED rings will turn yellow and move the head up and down as if he was an actor
  writeBluetoothOthers("THANKS", false);
  //enable left arm
  ArmL.enableOutputs();
  ArmL.setCurrentPosition(0);
  //lift left arm
  ArmL.moveTo(600);
  while (ArmL.distanceToGo() != 0) {
    armL();
  }
  //enable forearm
  ForeArmL.enableOutputs();
  //change once more the max speed and acceleration for the left forearm
  ForeArmL.setMaxSpeed(2000);
  ForeArmL.setAcceleration(2000);
  //Rigel moves his forearm to the left and to the right twice, waving to say goodbye to everyone
  for (int j = 0; j < 2; j++) {
    ForeArmL.setCurrentPosition(0);
    ForeArmL.moveTo(-600);
    while (ForeArmL.distanceToGo() != 0) {
      Serial.println(analogRead(botForearmL));
      forearmL();
    }
    ForeArmL.setCurrentPosition(0);
    ForeArmL.moveTo(600);
    while (ForeArmL.distanceToGo() != 0) {
      forearmL();
    }
  }
  //the left arm and forearm go back to the place
  resetArms();
  delay(2000);
  //Rigel turns 90 degrees
  NoventaDegrees();
  //left arm lifts again
  ArmL.enableOutputs();
  ArmL.setCurrentPosition(0);
  ArmL.moveTo(600);
  while (ArmL.distanceToGo() != 0) {
    armL();
  }
  delay(1000);
  //the head turns 60 degrees to the crowd
  turnHead(60);
  //sets again new max speed and acceleration for the wheels and Rigel leaves the stage holding Chaplin's hand
  stepperL.setMaxSpeed(400);  
  stepperR.setMaxSpeed(400);
  stepperL.setAcceleration(300);
  stepperR.setAcceleration(300);
  stepperL.enableOutputs();
  stepperR.enableOutputs();
  stepperL.setCurrentPosition(0);
  stepperR.setCurrentPosition(0);
  stepperL.moveTo(3000);
  stepperR.moveTo(3000);
  //This would stop the movement, however, the robot will probably be turned off before this moments comes as the presentation ends as described above
  bool stop = false;
  while (!stop) {
    stepperL.run();
    stepperR.run();
    if (stepperR.distanceToGo() == 0 && stepperL.distanceToGo() == 0) stop = true;
  }
}