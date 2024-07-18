//This is the void for when Chaplin removes Rigel's head
void noMoreHead() {

  delay(1000);
  //send "OFF" to the head, meaning that the LED rings will turn off
  writeBluetoothOthers("OFF", false);
  delay(6000);
  //send "WHITE" to the head, meaning that the LED rings will turn white
  writeBluetoothOthers("WHITE", false);
  //send "TRACK" to the head, meaning that the head will start tracking and facing Chaplin's face
  writeBluetoothOthers("TRACK", false);
  delay(10000);
  //send "MAD" to the head, meaning that the LED rings will turn red
  writeBluetoothOthers("MAD", false);
  delay(3000);
  //both arms get enabled and lifted
  ArmR.enableOutputs();
  ArmL.enableOutputs();
  ArmR.setCurrentPosition(0);
  ArmL.setCurrentPosition(0);
  ArmR.moveTo(350);
  ArmL.moveTo(400);
  bool stopMovement = false;
  while (!stopMovement) {
    armL();
    armR();
    if (ArmR.distanceToGo() == 0 && ArmL.distanceToGo() == 0) stopMovement = true;
  }
  //the robot turns 90 degrees
  NoventaDegrees();
  //stepLeft and stepRight are reseted
  stepLeft = 0;
  stepRight = 0;
  //the motors for the wheels are enabled
  stepperR.enableOutputs();
  stepperL.enableOutputs();
  //the file named "chase.bin" is opened in the SD card
  bin = SD.open("chase.bin");
  //dt metro count resets
  dt.reset();
  //timePass resets
  timePass = 0;
  //the robot makes the desired trajectory
  while (makeBin()) {
  }
  //the file is closed
  bin.close();
  //both the wheel's motors get disabled
  stepperR.disableOutputs();
  stepperL.disableOutputs();
  delay(2000);
  //send "SAD" to the head, meaning that the LED rings will turn blue
  writeBluetoothOthers("SAD", false);
  //the arms go back to their orignal position
  resetArms();
  //Chaplin gives back Rigel's head to leave this while
  int leituraLDR = analogRead(ldrPin);
  while (leituraLDR > 200) {
    leituraLDR = analogRead(ldrPin);
  }
  //send "HAPPY" to the head, meaning that the LED rings will turn yellow
  delay(2000);
  writeBluetoothOthers("HAPPY", false);
  //the head goes back to its original position
  resetHead();
  delay(2000);
}