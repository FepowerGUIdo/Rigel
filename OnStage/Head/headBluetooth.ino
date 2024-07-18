//This void reads what is coming from the body bluetooth 
void readBluetooth() {
  //resets the string
  bluetoothCommand = "";
  if (Serial3.available()) {
    //the string is now what is being read at the Serial port of the bluetooth module 
    bluetoothCommand = Serial3.readStringUntil('\n');
    //if printOrNot is true(which means that we would be testing the communcation) then the bluetoothCommand shall be printed at the Serial Monitor
    if (printOrNot) Serial.println(bluetoothCommand);
    //checks if the bluetooth is connected by verifying if its stat is 1
    if (bluetoothCommand == "+STAT=1\r") {
      isBlueConnected = true;
    }
    //checks if the bluetooth is disconnected
    if (bluetoothCommand == "+STAT=0\r") {
      isBlueConnected = false;
    }
    //checks if the bluetooth disconnected/should disconnect for any reason
    if(bluetoothCommand == "+DISCONNECT=1\r") {
      writeBluetooth("AT+DISC");
    }
    //checks if the bluetooth is connected back
    if(bluetoothCommand == "+CONNECTED=1\r") {
      isBlueConnected = true;
    }
  }
  if (resetBluetooth.check() == true) {
    resetBluetooth.reset();
    writeBluetooth("AT+STAT");
    if (isBlueConnected == false) {
      delay(20);
      writeBluetooth("AT+BAND11899AA226E1");
    }
  }
}


void writeBluetooth(String writeToBT) {
  Serial3.println(writeToBT);
}

void writeBluetoothOthers(String writeToBT) {
  String atData = "AT+DATA1";
  atData.concat(writeToBT);
  Serial3.println(atData);
  Serial.println(atData);
}

void configBluetooth() {
  if (Serial.available()) {
    leituraSerial = Serial.readStringUntil('\n');
    Serial3.println(leituraSerial);
  }
}
