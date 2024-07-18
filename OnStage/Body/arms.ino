//This void resets the position of the arms
void resetArms() {

  //change the parameters for max speed and acceleration for the arms and forearms
  ArmL.setMaxSpeed(1000);
  ArmL.setAcceleration(200);
  ArmR.setMaxSpeed(1000);
  ArmR.setAcceleration(200);
  ForeArmL.setMaxSpeed(200);
  ForeArmL.setAcceleration(200);
  ForeArmR.setMaxSpeed(200);
  ForeArmR.setAcceleration(200);

  //Move both forearms towards the standard position
  ForeArmL.moveTo(2000);
  ForeArmR.moveTo(2000);
  //creates 2 bools to verify if the forearms are on their standard position
  bool foreArmLReseted = false;
  bool foreArmRReseted = false;
  //Enable the forearms
  ForeArmL.enableOutputs();
  ForeArmR.enableOutputs();
  while (1) {
    //creates a variable to store the analog read of the forearm left "switch", then, if this read is < 350(meaning that the standard position was reached), the forearm should stop and it is now reseted
    int leituraBotForeArmL = analogRead(botForearmL);
    ForeArmL.run();
    if (leituraBotForeArmL < 350 && ForeArmL.distanceToGo() >= 0) {
      ForeArmL.setSpeed(0);
      ForeArmL.runSpeed();
      ForeArmL.moveTo(0);
      ForeArmL.setCurrentPosition(0);
      foreArmLReseted = true;
    }
    //creates a variable to store the analog read of the forearm right "switch", then, if this read is < 350(meaning that the standard position was reached), the forearm should stop and it is now reseted
    int leituraBotForeArmR = analogRead(botForearmR);
    ForeArmR.run();
    if (leituraBotForeArmR < 350 && ForeArmR.distanceToGo() >= 0) {
      ForeArmR.setSpeed(0);
      ForeArmR.runSpeed();
      ForeArmR.moveTo(0);
      ForeArmR.setCurrentPosition(0);
      foreArmRReseted = true;
    }
    if (foreArmRReseted == true && foreArmLReseted == true) {
      break;
    }
  }
  //the forearms are now disabled
  ForeArmL.disableOutputs();
  ForeArmR.disableOutputs();
  //the arms are enabled
  ArmL.enableOutputs();
  ArmR.enableOutputs();
  //they start moving towards the standard postion
  ArmL.moveTo(-2000);
  ArmR.moveTo(-2000);
  //creates 2 bools to verify if the arms are on their standard position
  bool ArmLReseted = false;
  bool ArmRReseted = false;
  while (1) {
    //creates a variable to store the analog read of the arm left switch, then, if this read is < 350(meaning that the standard position was reached), the arm should stop and it is now reseted
    int leituraBotArmL = analogRead(botArmL);
    if (printOrNot) {
      Serial.print("leitura BotArmL");
      Serial.println(leituraBotArmL);
    }
    ArmL.run();
    if (leituraBotArmL < 350 && ArmL.distanceToGo() <= 0) {
      ArmL.setSpeed(0);
      ArmL.runSpeed();
      ArmL.moveTo(0);
      ArmL.setCurrentPosition(0);
      ArmLReseted = true;
    }
    //creates a variable to store the analog read of the arm right, then, if this read is < 350(meaning that the standard position was reached), the arm should stop and it is now reseted
    int leituraBotArmR = analogRead(botArmR);
    if (printOrNot) {
      Serial.print("leitura BotArmR");
      Serial.println(leituraBotArmR);
    }
    ArmR.run();
    if (leituraBotArmR < 350 && ArmR.distanceToGo() <= 0) {
      ArmR.setSpeed(0);
      ArmR.runSpeed();
      ArmR.moveTo(0);
      ArmR.setCurrentPosition(0);
      ArmRReseted = true;
    }
    //if both arms have returned to their standar position, the algorithm may now leave the while loop
    if (ArmRReseted == true && ArmLReseted == true) {
      break;
    }
  }
  //arms are now disabled
  ArmL.disableOutputs();
  ArmR.disableOutputs();

  //max speed and acceleration are changed for the arms and forearms
  ArmL.setMaxSpeed(3000);
  ArmL.setAcceleration(800);
  ArmR.setMaxSpeed(3000);
  ArmR.setAcceleration(800);
  ForeArmL.setMaxSpeed(2000);
  ForeArmL.setAcceleration(800);
  ForeArmR.setMaxSpeed(2000);
  ForeArmR.setAcceleration(800);
}

//This void moves the left arm
void armL() {
  //creates a local variable again to store the analogRead for the left arm switch
  int leituraBotArmL = analogRead(botArmL);
  ArmL.run();
  /*if (printOrNot) {
    Serial.print("BotArmL: ");
    Serial.println(leituraBotArmL);
  }*/
  //Basically run the motors untill they touch the top or the bottom switch
  if (leituraBotArmL > 600 && ArmL.distanceToGo() >= 0) {
    ArmL.setSpeed(0);
    ArmL.runSpeed();
    ArmL.moveTo(0);
    ArmL.setCurrentPosition(0);
  }
  if (leituraBotArmL < 350 && ArmL.distanceToGo() <= 0) {
    ArmL.setSpeed(0);
    ArmL.runSpeed();
    ArmL.moveTo(0);
    ArmL.setCurrentPosition(0);
  }
}

//This void movers right arm
void armR() {
  //creates a local variable again to store the analogRead for the right arm switch
  int leituraBotArmR = analogRead(botArmR);
  if (printOrNot) {
    Serial.print("BotArmR: ");
    Serial.println(leituraBotArmR);
  }
  //Basically run the motors untill they touch the top or the bottom switch
  ArmR.run();
  if (leituraBotArmR > 600 && ArmR.distanceToGo() >= 0) {
    ArmR.setSpeed(0);
    ArmR.runSpeed();
    ArmR.moveTo(0);
    ArmR.setCurrentPosition(0);
  }
  if (leituraBotArmR < 350 && ArmR.distanceToGo() <= 0) {
    ArmR.setSpeed(0);
    ArmR.runSpeed();
    ArmR.moveTo(0);
    ArmR.setCurrentPosition(0);
  }
}

//This void moves the left forearm
void forearmL() {
  //creates a local variable again to store the analogRead for the left forearm "switch"
  int leituraBotForeArmL = analogRead(botForearmL);
  if (printOrNot) {
    Serial.print("BotForeArmL: ");
    Serial.println(leituraBotForeArmL);
  }
  //Basically run the motors untill they touch the left or the right switch
  ForeArmL.run();
  if (leituraBotForeArmL > 600 && ForeArmL.distanceToGo() <= 0) {
    ForeArmL.setSpeed(0);
    ForeArmL.runSpeed();
    ForeArmL.moveTo(0);
    ForeArmL.setCurrentPosition(0);
  }
  if (leituraBotForeArmL < 350 && ForeArmL.distanceToGo() >= 0) {
    ForeArmL.setSpeed(0);
    ForeArmL.runSpeed();
    ForeArmL.moveTo(0);
    ForeArmL.setCurrentPosition(0);
  }
}

void forearmR() {
  //creates a local variable again to store the analogRead for the left forearm "switch"
  int leituraBotForeArmR = analogRead(botForearmR);
  if (printOrNot) {
    Serial.print("BotForeArmR: ");
    Serial.println(leituraBotForeArmR);
  }
  ForeArmR.run();
  //Basically run the motors untill they touch the left or the right switch
  if (leituraBotForeArmR > 600) {
    ForeArmR.setSpeed(0);
    ForeArmR.runSpeed();
    ForeArmR.moveTo(0);
    ForeArmR.setCurrentPosition(0);
  }
  if (leituraBotForeArmR < 350 && ForeArmR.distanceToGo() >= 0) {
    ForeArmR.setSpeed(0);
    ForeArmR.runSpeed();
    ForeArmR.moveTo(0);
    ForeArmR.setCurrentPosition(0);
  }
}