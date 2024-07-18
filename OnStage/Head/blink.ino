//This void will make the eyes blink by turning them on and off repeatedly 
void blink() {
  //turn on all the 16 dots at "max white"
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 255, 255, 255);
    strip2.setPixelColor(i, 255, 255, 255);
    strip.show();
    strip2.show();
  }
  delay(100);
  //turns off all the 16 dots
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 0, 0, 0);
    strip2.setPixelColor(i, 0, 0, 0);
    strip.show();
    strip2.show();
  }
  //repeats the same process 5 more times:
  delay(100);
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 255, 255, 255);
    strip2.setPixelColor(i, 255, 255, 255);
    strip.show();
    strip2.show();
  }
  delay(100);
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 0, 0, 0);
    strip2.setPixelColor(i, 0, 0, 0);
    strip.show();
    strip2.show();
  }
  delay(100);
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 255, 255, 255);
    strip2.setPixelColor(i, 255, 255, 255);
    strip.show();
    strip2.show();
  }
  delay(100);
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 0, 0, 0);
    strip2.setPixelColor(i, 0, 0, 0);
    strip.show();
    strip2.show();
  }
  delay(100);
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 255, 255, 255);
    strip2.setPixelColor(i, 255, 255, 255);
    strip.show();
    strip2.show();
  }
  delay(100);
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 0, 0, 0);
    strip2.setPixelColor(i, 0, 0, 0);
    strip.show();
    strip2.show();
  }
  delay(100);
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 255, 255, 255);
    strip2.setPixelColor(i, 255, 255, 255);
    strip.show();
    strip2.show();
  }
  delay(100);
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 0, 0, 0);
    strip2.setPixelColor(i, 0, 0, 0);
    strip.show();
    strip2.show();
  }
  delay(100);
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 255, 255, 255);
    strip2.setPixelColor(i, 255, 255, 255);
    strip.show();
    strip2.show();
  }
  delay(100);
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 0, 0, 0);
    strip2.setPixelColor(i, 0, 0, 0);
    strip.show();
    strip2.show();
  }
}