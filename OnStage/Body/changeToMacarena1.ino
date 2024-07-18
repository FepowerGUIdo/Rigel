//This void is to make Rigel dance Macarena
void changeToMacarena1() {
  //send "HAPPY" to the head, meaning that the LED rings will turn yellow
  writeBluetoothOthers("HAPPY", false);
  delay(5000);
  //send "NAUGHTY" to the head, meaning that the LED rings will turn purple
  writeBluetoothOthers("NAUGHTY", false);
  delay(2000);
  //send "HAPPY" to the head, meaning that the LED rings will turn yellow
  writeBluetoothOthers("HAPPY", false);
  //enables the arms and forearms
  ArmR.enableOutputs();
  ArmL.enableOutputs();
  ForeArmR.enableOutputs();
  ForeArmL.enableOutputs();

  //sets new max speed and acceleration for all of them
  ForeArmR.setMaxSpeed(2000);
  ForeArmR.setAcceleration(3000);
  ForeArmL.setMaxSpeed(2000);
  ForeArmL.setAcceleration(3000);
  ArmL.setMaxSpeed(2000);
  ArmL.setAcceleration(3000);
  ArmR.setMaxSpeed(2000);
  ArmR.setAcceleration(3000);

  //initiates a bool that will say when to stop
  bool stop = false;

  //lift right arm
  ArmR.setCurrentPosition(0);
  ArmR.moveTo(300);
  while (ArmR.distanceToGo() != 0) {
    armR();
  }

  //lift left arm
  ArmL.setCurrentPosition(0);
  ArmL.moveTo(350);
  while (ArmL.distanceToGo() != 0) {
    armL();
  }

  //"close" right forearm
  ForeArmR.setCurrentPosition(0);
  ForeArmR.moveTo(-900);
  while (ForeArmR.distanceToGo() != 0) {
    forearmR();
  }

  //"close" left forearm
  ForeArmL.setCurrentPosition(0);
  ForeArmL.moveTo(-900);
  while (ForeArmL.distanceToGo() != 0) {
    forearmL();
  }

  //"open" right forearm
  ArmR.setCurrentPosition(0);
  ArmR.moveTo(300);
  ForeArmR.setCurrentPosition(0);
  ForeArmR.moveTo(900);
  while (!stop) {
    armR();
    forearmR();
    if (ArmR.distanceToGo() == 0 && ForeArmR.distanceToGo() == 0) stop = true;
  }
  //"open left forearm"
  stop = false;
  ArmL.setCurrentPosition(0);
  ArmL.moveTo(300);
  ForeArmL.setCurrentPosition(0);
  ForeArmL.moveTo(900);
  while (!stop) {
    armL();
    forearmL();
    if (ArmL.distanceToGo() == 0 && ForeArmL.distanceToGo() == 0) stop = true;
  }
  //Macarena is over :)
  //the arms and forearms get back to standard position 
  resetArms();

  //the max speed and accelerations are again changed back
  ArmL.setMaxSpeed(1000);
  ArmL.setAcceleration(200);
  ArmR.setMaxSpeed(1000);
  ArmR.setAcceleration(200);
  ForeArmL.setMaxSpeed(1000);
  ForeArmL.setAcceleration(200);
  ForeArmR.setMaxSpeed(1000);
  ForeArmR.setAcceleration(200);
}