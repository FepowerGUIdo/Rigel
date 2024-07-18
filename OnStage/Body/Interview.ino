//This is a void developed for the technical interview, it allows us to easily demonstrate our features and glove commands by setting the head removal as the trigger to unlock the glove
void interview() {
  delay(1000);
  while (1) {
    int leituraLDR = analogRead(ldrPin);
    if (leituraLDR > 100) {
      WaitingForGesture(2);
    }
  }
}