void readBluetooth() {
  if (Serial8.available()) {
    leituraBT = Serial8.readStringUntil('\n');
    if (printOrNot) Serial.println(leituraBT);
    if(leituraBT=="G4"){
      Serial.println("ola");
    }
  }
}

void writeBluetooth(String writeToBT) {
  Serial8.println(writeToBT);
  delay(10);
}

void writeBluetoothOthers(String writeToBT, bool isGlove) {
  String atData1 = "AT+DATA";
  String atData2 = "AT+DATA";
  atData1.concat(1).concat(writeToBT).concat('\n');
  Serial8.println(atData1);
  delay(20);
  atData2.concat(2).concat(writeToBT).concat('\n');
  Serial8.println(atData2);
  delay(20);
  if(printOrNot) Serial.println(atData1);
}

void configBluetooth() {
  if (Serial.available()) {
    leituraSerial = Serial.readStringUntil('\n');
    Serial8.println(leituraSerial);
  }
  delay(10);
}
