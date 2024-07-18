//This void is called whenver the body should recieve a gesture from the glove
//Count gesture is a variable that we created to avoid the robot from leaving the stage in case of a wrong interepretation of the command
void WaitingForGesture(int countGesture) {
  //initiates a bool to check if we are really waiting for something
  bool waitingOkay = false;
  //initiates the metro instance
  Metro resendG = Metro(1000);
  resendG.check();
  //"tells" to the glove that the body is waiting for something
  writeBluetoothOthers("WAITING", true);
  //initiates a millis instance
  long int startingTime = millis();
  //resets the bluetooth reading
  leituraBT = "";
  //initiates a loop
  while (1) {
    //reads the incoming string from the glove
    readBluetooth();
    //if it is thumbs up
    if (leituraBT == "G1") {
      //sends "okay" to the glove, meaning that the string has arrived
      writeBluetoothOthers("OKAY", true);
      delay(10);
      //enable the arms steppers
      ArmL.enableOutputs();
      ArmL.setCurrentPosition(0);
      ArmL.moveTo(350);
      bool stopMovement = false;
      //raise the arm
      while (!stopMovement) {
        armL();
        if (ArmL.distanceToGo() == 0) stopMovement = true;
        if (ArmL.distanceToGo() == 150) {
          //sends "YES" to the head, meaning the LED rings will turn green
          writeBluetoothOthers("YES", false);
        }
      }
      //do the thumbs up
      thumbsUp();
      delay(2000);
      //leave the thumbs up
      notThumbsUp();
      //withdraw the arm
      resetArms());
      delay(2000);
      break;
    }
    //if it is call
    if (leituraBT == "G2") {
      //sends "OKAY" to the glove, meaning that the string has arrived
      writeBluetoothOthers("OKAY", true);
      delay(10);
      //sends "ORANGE" to the head, making the LED rings turn orange
      writeBluetoothOthers("ORANGE", false);
      //Since the call command makes Rigel move forward, we really need to be sure that he will only perform this command when needed, otherwise it could leave the stage and this would not be funny
      //Thus, this if checks if "call" is being sent as the 2nd or 5th command
      if (countGesture == 2 || countGesture == 5) {
        //enables both wheels motors
        stepperR.enableOutputs();
        stepperL.enableOutputs();
        //opens "call1.bin" file on the SD card
        bin = SD.open("call1.bin");
        //resets timePass, dt, stepLeft and stepRight
        stepLeft = 0;
        stepRight = 0;
        dt.reset();
        timePass = 0;
        //performs the trajectory
        while (makeBin()) {
        }
        //closes the file
        bin.close();
        //disable both motors
        stepperR.disableOutputs();
        stepperL.disableOutputs();
        delay(2000);
        else {
          delay(5000);
        }
        break;
      }
      //if it is rewind
      if (leituraBT == "G3") {
        //sends "OKAY" to the glove, meaning that the string has arrived
        writeBluetoothOthers("OKAY", true);
        delay(10);
        //sends "BLINK" to the head, meaning that the eyes will start blinking
        writeBluetoothOthers("BLINK", false);
        delay(5000);
        break;
      }
      //if it is hello
      if (leituraBT == "G4") {
        //sets new max speed and acceleration for the Forearm left
        ForeArmL.setMaxSpeed(2000);
        ForeArmL.setAcceleration(3000);
        //sends "OKAY" to the glove, meaning that the string has arrived
        writeBluetoothOthers("OKAY", true);
        delay(10);
        //sends "HAPPY" to the head, meaning that the LED rings will turn yellow
        writeBluetoothOthers("HAPPY", false);
        //enable arm left
        ArmL.enableOutputs();
        ArmL.setCurrentPosition(0);
        //raise arm left
        ArmL.moveTo(600);
        while (ArmL.distanceToGo() != 0) {
          armL();
        }
        //enable forearm left
        ForeArmL.enableOutputs();
        ForeArmL.setMaxSpeed(2000);
        ForeArmL.setAcceleration(2000);
        //wave with the forearm, saying hello
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
        //resets both the arm and forearm 
        resetArms();
        delay(2000);
        break;
      }
      //In case everything goes wrong and the glove for some reason decides to stop working, there is still a solution
      //if Rigel keeps 15 seconds in this void without recieving a command, he will get mad at you 
      if (millis() - startingTime > 15000) {
        //sends "MAD" to the head, meaning that the LED rings will turn red
        writeBluetoothOthers("MAD", false);
        turnHead(45);
        turnHead(-90);
        turnHead(90);
        turnHead(-45);
        break;
        //Não
      }
      //finally, checks if the time to resend a gesture has past and if Rigel is not waiting, in this case, Rigel will wait 
      if (resendG.check() == 1 && waitingOkay == false) {
        writeBluetoothOthers("WAITING", true);
      }
      if (leituraBT == "OKAY") {
        waitingOkay = true;
      }
    }
  }