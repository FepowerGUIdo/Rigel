//Void that runs at the start of the presentation (careless whisper)
void start() {
  //The LED rings turns pink
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 255, 120, 218);
    strip2.setPixelColor(i, 255, 120, 218);
    strip.show();
    strip2.show();
  }
}