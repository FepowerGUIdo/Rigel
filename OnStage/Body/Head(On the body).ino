//This void sets the head to the standard position
void resetHead() {
  //Enables the motor and sets a speed
  Head.enableOutputs();
  Head.setSpeed(200);
  //makes the head spin until the LDR detects the heads laser straight up, meaning that the head achieved its standard point, stopping then the movement
  while (1) {
    int leituraLDR = analogRead(ldrPin);
    if (printOrNot) {
      Serial.print("leituraLDR: ");
      Serial.println(leituraLDR);
    }
    if (leituraLDR > 900) {
      Head.setSpeed(0);
      break;
    }
    Head.runSpeed();
  }
  Head.disableOutputs();
}
