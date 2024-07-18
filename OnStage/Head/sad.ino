//This void makes Rigel sad
void sad() {
  //The LED rings turn blue
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 0, 0, 255);
    strip2.setPixelColor(i, 0, 0, 255);
    strip.show();
    strip2.show();
  }
  //the 3 servos are attached
  Mouth.attach(49);
  EyebrowR.attach(51);
  EyebrowL.attach(53);
  //they are set to the "common sad" face
  Mouth.write(175);
  EyebrowL.write(98);
  EyebrowR.write(80);
    delay(1000);
  //the servos are detached
  Mouth.detach();
  EyebrowR.detach();
  EyebrowL.detach();
}
