//This void turns the LED rings orange
void orange() {
  //The LED rings become orange
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 242, 133, 0);
    strip2.setPixelColor(i, 242, 133, 0);
    strip.show();
    strip2.show();
  }
}