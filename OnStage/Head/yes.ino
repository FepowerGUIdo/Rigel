//This void makes Rigel nod and do a thumbs up
void yes() {
  //The LED rings turn green
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 0, 255, 0);
    strip2.setPixelColor(i, 0, 255, 0);
    strip.show();
    strip2.show();
  }
  //The nod system is similar to the arms waving system 
  //The head starts moving until it reaches the switch
  HeadMotor.enableOutputs();
  HeadMotor.moveTo(-2000);
  int countButtonPress = 0;
  while (countButtonPress < 4) {
    if (!digitalRead(3)) {
      // Move head upward if it's not already moving
      if (HeadState != Head_Moving_Upward) {
        //Now the head has touched the bottom switch and started to move upward
        Serial.println("3");
        HeadMotor.setSpeed(0);
        HeadMotor.runSpeed();
        HeadMotor.moveTo(-5000);
        countButtonPress++;
        HeadState = Head_Moving_Upward;
      }
    }

    // Check contact 2
    if (!digitalRead(2)) {
      // Move head backward if it's not already moving
      if (HeadState != Head_Moving_Downward) {
        //Now the head touched the top switch and will move downward
        Serial.println("2");
        HeadMotor.setSpeed(0);
        HeadMotor.runSpeed();
        HeadMotor.moveTo(5000);
        countButtonPress++;
        HeadState = Head_Moving_Downward;
      }
    }

    // Update head state and run motor
    if (HeadState != HeadIdle) {
      // Check if head has reached the end position
      if (HeadMotor.distanceToGo() == 0) {
        // Head has stopped moving
        HeadState = HeadIdle;
        HeadMotor.setCurrentPosition(0);
      }
    }
    HeadMotor.run();
  }
  //puts head back to the initial position
  HeadMotor.setCurrentPosition(0);
  HeadMotor.moveTo(-120);
  while(HeadMotor.distanceToGo()!=0){
    HeadMotor.run();
  }
  //disable the motor to save energy and resets the eyebrows, mouth and eye color
  HeadMotor.disableOutputs();
    normalize();
}