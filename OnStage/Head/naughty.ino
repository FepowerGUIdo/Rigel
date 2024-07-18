//This void makes the robot do a naughty face
void naughty() {
  //sets the LED ring to purple
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 160, 32, 240);
    strip2.setPixelColor(i, 160, 32, 240);
    strip.show();
    strip2.show();
  }
  //attach the 3 servos
  Mouth.attach(49);
  EyebrowR.attach(51);
  EyebrowL.attach(53);
  //make them go to a "common naughty look"
  Mouth.write(40);
  EyebrowL.write(73);
  EyebrowR.write(105);
  delay(1000);
  //detaches the servos to save energy
  Mouth.detach();
  EyebrowR.detach();
  EyebrowL.detach();
}