void track() {
  if (Serial1.available()) {
    int16_t center_y;
    char teste[2];
    Serial1.readBytes((char*)&center_y, 2);
    Serial.println(center_y);

    if (center_y > 90 && center_y < 161) {
      HeadMotor.setSpeed(100);
    }

    else if (center_y < 70) {
      HeadMotor.setSpeed(-100);
    }

    else if (center_y >= 70 && center_y <= 90) {
      HeadMotor.setSpeed(0);
    }

    if (!digitalRead(3)) {
      // Move head forward if it's not already moving
      if (HeadMotor.speed()>0) {
        Serial.println("3");
        HeadMotor.setSpeed(0);
      }
    }

    // Check contact 2
    if (!digitalRead(2)) {
      // Move head backward if it's not already moving
      if (HeadMotor.speed()<0) {
        Serial.println("2");
        HeadMotor.setSpeed(0);
      }
    }
    
  }
  HeadMotor.runSpeed();
}

