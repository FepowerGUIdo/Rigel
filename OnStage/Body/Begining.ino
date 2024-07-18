//Void startingSetup makes the body connect with the glove and the head + resets the position of the hand and the arms
void startingSetup() {
  //resets the bluetooth module JDY-24M
  writeBluetooth("AT+RESET");
  //Opens Rigel's doors to put the bluetooth acessory back inside
  openDoors();
  //resets the position of the fingers and the arms
  notThumbsUp();
  resetArms();
  delay(200);

  //Keeps the robot in this while until the head is connected to both the glove and the head(that is why "+CONNECTED=2\r")
  while (leituraBT != "+CONNECTED=2\r") {
    //Calls voids to connect the modules
    readBluetooth();
    configBluetooth();
  }
}

//This void represents the begining of our presentation, after all the necessary resets were made
void Begining() {
  //leituraLDR is a variable that we are creating to store the value for our LDR, again, leitura is read in portuguese, and we chose to work with it as "read" might conflict with other IDE functions
  int leituraLDR = analogRead(ldrPin);
  //creates a while that will keep stuck until the head is put in the robot, which will trigger the start of the presentation
  while (leituraLDR > 100) {
    leituraLDR = analogRead(ldrPin);
    if (printOrNot) Serial.println(leituraLDR);
  }
  //The doors now close
  closeDoors();
  delay(34000);
  //after 34 seconds of preparing to start, the head now go to its standard position
  resetHead();
  //write "START" to the head bluetooth(we know that this is going to the head because of the "false" parameter on the void, this will be further explained in the bluetooth section)
  writeBluetoothOthers("START", false);
  //Resets the variables for stepLeft and stepRight
  stepLeft = 0;
  stepRight = 0;
  //Enables back the wheels and head motors
  stepperR.enableOutputs();
  stepperL.enableOutputs();
  Head.enableOutputs();
  //set the Head motor current position as 0
  Head.setCurrentPosition(0);
  //opens on the SD card a file named "beginning.bin", this is our initial trajectory
  bin = SD.open("beginning.bin");
  //resets the dt metro and timePass variable
  dt.reset();
  timePass = 0;
  //starts a millis instance
  long int beginTime = millis();
  //calls the void makeBin, that runs the previously opened trajectory
  while (makeBin()) {
    //makes the head turn as time passes in the trajectory, this gives more emotion to the robot
    if (millis() - beginTime > 4000) {
      Head.moveTo((-60 / 360.0) * (2600));
    }
    if (millis() - beginTime > 2000) {
      Head.moveTo((-45 / 360.0) * (2600));
    }
    Head.run();
  }
  //close the opened file for the trajectory
  bin.close();
  //disables all the motors again
  Head.disableOutputs();
  stepperR.disableOutputs();
  stepperL.disableOutputs();
  //sends "NAUGHTY" to the head, this will change Rigel's eye color and expression
  writeBluetoothOthers("NAUGHTY", false);
  //open doors again so that Chaplin can now pick the bluetooth ornament
  openDoors();
  //gives time to chaplin pick it and react to it
  delay(2000);
  delay(10000);
  //close back the door
  closeDoors();
  delay(2000);
  //turns 90 degrees
  NoventaDegrees();
  //?
  Head.enableOutputs();
  Head.moveTo(0);
  while(Head.distanceToGo()!=0){
    Head.run();
  }
  Head.disableOutputs();
}