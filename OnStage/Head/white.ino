//This void makes Rigel's eyes white
void white() {
  //The LED rings turn white
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 255, 255, 255);
    strip2.setPixelColor(i, 255, 255, 255);
    strip.show();
    strip2.show();
  }
}

//This void makes Rigel's eyes white, but not that bright, it spends less energy
void white2() {
  //The LED rings reduce the brightness of the white light
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 100, 100, 100);
    strip2.setPixelColor(i, 100, 100, 100);
    strip.show();
    strip2.show();
  }
}