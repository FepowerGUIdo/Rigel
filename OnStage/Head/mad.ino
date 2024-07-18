//This void makes Rigel look mad
void mad() {
  //the LED rings becomes red
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 255, 0, 0);
    strip2.setPixelColor(i, 255, 0, 0);
    strip.show();
    strip2.show();
  }
  //the 3 servos are attached
  Mouth.attach(49);
  EyebrowR.attach(51);
  EyebrowL.attach(53);
  //they are moved to the "common mad face" position 
  Mouth.write(170);
  EyebrowL.write(78);
  EyebrowR.write(100);
    delay(1000);
  //the servos are detached
  Mouth.detach();
  EyebrowR.detach();
  EyebrowL.detach();
}