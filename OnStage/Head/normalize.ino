//this void makes everything go to the standard position
void normalize() {
  //the LED rings are turned off
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 0, 0, 0);
    strip2.setPixelColor(i, 0, 0, 0);
    strip.show();
    strip2.show();
  }
  //mouth and eyebrows are atached and set to the idle position
  Mouth.attach(49);
  EyebrowR.attach(51);
  EyebrowL.attach(53);
  Mouth.write(10);
  EyebrowR.write(90);
  EyebrowL.write(88);
  delay(500);
  //servos are detached 
  Mouth.detach();
  EyebrowL.detach();
  EyebrowR.detach();
}