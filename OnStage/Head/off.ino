//this void turns off the LED rings
void off() {
  //The LED rings are turned off
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 0, 0, 0);
    strip2.setPixelColor(i, 0, 0, 0);
    strip.show();
    strip2.show();
  }
}