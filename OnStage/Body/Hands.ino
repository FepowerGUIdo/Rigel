//This void makes the robot do a thumbs up
void thumbsUp(){
  //attach the servos
  handL.attach(servoHandLPin);
  handR.attach(servoHandRPin);
  //make them move, performing a thumbs up
  handL.write(170);
  handR.write(170);
}
//This void stops the thumbs up
void notThumbsUp(){
  //attach the servos, stopping the thumbs up
  handL.attach(servoHandLPin);
  handR.attach(servoHandRPin);
  //make them move, stopping the thumbs up
  handL.write(10);
  handR.write(10);
  delay(500);
  //detach the servos in order save energy
  handR.detach();
  handL.detach();

}