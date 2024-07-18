//This void makes Rigel look like he is happy
void happy() {
  //This for makes his eyes turn yellow
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 255, 255, 0);
    strip2.setPixelColor(i, 255, 255, 0);
    strip.show();
    strip2.show();
  }
  //attaches all the 3 servos
  Mouth.attach(49);
  EyebrowR.attach(51);
  EyebrowL.attach(53);
  //sends all of them to the "common position" for a happy face
  Mouth.write(10);
  EyebrowL.write(98);
  EyebrowR.write(80);
    delay(1000);
  //detach all of them to save energy
  Mouth.detach();
  EyebrowR.detach();
  EyebrowL.detach();
}