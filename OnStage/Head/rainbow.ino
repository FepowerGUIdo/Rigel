//This void makes the LED rings colorful and the colors spin
void rainbow()
{
  //This void keeps alterning the colors of the LED rings, making a effect in the eyes that look like a rainbow
  for (int i = 0; i < 16; i++)
  {
    Rrainbow = (Rrainbow + 255 / 16) % 255;
    Grainbow = (Grainbow + 255 / 16) % 255;
    Brainbow = (Brainbow + 255 / 16) % 255;
    strip.setPixelColor(i, Rrainbow, Grainbow, Brainbow);
    strip2.setPixelColor(16 - i - 1, Rrainbow, Grainbow, Brainbow);
    strip.show();
    strip2.show();
    delay(1);
  }
}