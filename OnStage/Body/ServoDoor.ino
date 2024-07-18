//This void opens Rigel's doors to give chaplin the bluetooth adornment
void openDoors() {
  //creates two bools to control wether or not both doors are open
  bool isDoorROpen = false;
  bool isDoorLOpen = false;
  //attach both door servos
  doorL.attach(servoDoorLPin);
  doorR.attach(servoDoorRPin);
  //make them move
  doorL.write(70);
  doorR.write(110);
  //generates a millis instance
  long int beginningTime = millis();
  while (1) {
  //reads both of the switches 
    int leituraBotDoorL = analogRead(botDoorL);
    int leituraBotDoorR = analogRead(botDoorR);
    //when switch 1 is pressed it means the left door opened
    if (leituraBotDoorL < 350) {
      doorL.detach();
      isDoorLOpen = true;
    }
    //when switch 2 is pressed it means the right door opened
    if (leituraBotDoorR < 350) {
      doorR.detach();
      isDoorROpen = true;
    }//if both doors are open, the algorithm can quit the while
    if (isDoorLOpen == true && isDoorROpen == true) {
      break;
    }
    //to guarantee that this will not compromise our presentation, if, for some reason, any of the servos do not work, the code will quit the while after 2 seconds
    if (millis() - beginningTime > 2000) break;
  }
  //both servos are now detached
  doorR.detach();
  doorL.detach();
}
//This void closes the doors 
void closeDoors() {
  //creates two bools to control wether or not both doors are closed
  bool isDoorRClosed = false;
  bool isDoorLClosed = false;
  //both servos are attached again
  doorL.attach(servoDoorLPin);
  doorR.attach(servoDoorRPin);
  //both of them move to close the doors
  doorL.write(110);
  doorR.write(70);
  //initiates another millis instance
  long int beginningTime = millis();
  while (1) {
    //reads both of the switches 
    int leituraBotDoorL = analogRead(botDoorL);
    int leituraBotDoorR = analogRead(botDoorR);
    //when switch 1 is pressed it means the left door closed
    if (leituraBotDoorL > 600) {
      doorL.detach();
      isDoorLClosed = true;
    }
    //when switch 2 is pressed it means the right door closed
    if (leituraBotDoorR > 600) {
      doorR.detach();
      isDoorRClosed = true;
    }
    //if both doors are closed, the algorithm can quit the while
    if (isDoorLClosed == true && isDoorRClosed == true) {
      break;
    }
    //to guarantee that this will not compromise our presentation, if, for some reason, any of the servos do not work, the code will quit the while after 2 seconds
    if (millis() - beginningTime > 2000) break;
  }
  //detach both servos
  doorR.detach();
  doorL.detach();
}