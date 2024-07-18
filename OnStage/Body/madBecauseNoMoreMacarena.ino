//This void is for when Rigel gets upset because Chaplin removed Macarena
void madBecauseNoMoreMacarena() {
  delay(5000);
  //send "MAD" to the head, meaning that the LED rings will turn red
  writeBluetoothOthers("MAD", false);
  //turns head 45 degrees
  turnHead(45);
  delay(2000);
  //turns head back
  turnHead(-45);
  //send "NAUGHTY" to the head, meaning that the LED rings will turn purple
  writeBluetoothOthers("NAUGHTY", false);
  delay(4000);
  //turn head 45 degrees
  turnHead(45);
  //send "HAPPY" to the head, meaning that the LED rings will turn yellow
  writeBluetoothOthers("HAPPY", false);
  delay(10);
  //Now Chaplin will remove Rigel's head, that is why the LDR read shoud be higer than 200
  int leituraLDR = analogRead(ldrPin);
  while (leituraLDR < 200) {
    leituraLDR = analogRead(ldrPin);
  }
  //send "OFF" to the head, meaning that the LED rings will turn OFF
  writeBluetoothOthers("OFF", false);
}