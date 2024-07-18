//This void makes Rigel thank the crowd with Chaplin
void thanks() {
  //The eyes turn yellow  
  for (int i = 0; i < 16; i++) {
    strip.setPixelColor(i, 255, 255, 0);
    strip2.setPixelColor(i, 255, 255, 0);
    strip.show();
    strip2.show();
  }
  //the head moves up and down until it reaches the switch, to make Rigel look like he is thanking the crowd, just like actors do at the end of performances
  HeadMotor.moveTo(5000);
  while (1) {
    HeadMotor.run();
    if (digitalRead(2)) {
      HeadMotor.setCurrentPosition(0);
      HeadMotor.moveTo(0);
      break;
    }
  }
  delay(1000);
  HeadMotor.moveTo(-5000);
  while (1) {
    HeadMotor.run();
    if (digitalRead(3)) {
      HeadMotor.setCurrentPosition(0);
      HeadMotor.moveTo(0);
    }
  }
}