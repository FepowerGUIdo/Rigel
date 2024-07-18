// Voids related to the state of the pulse from the encoder, "white" and "green" are due to the colors of the wires
void white1() {
  if (digitalRead(17)) {  // Checks the encoder pin
    stepLeft--;           // Decrements the stepLeft counter if the pulse goes from HIGH to LOW (rising edge)
  } else {
    stepLeft++;  // Increments the stepLeft counter if the pulse goes from LOW to HIGH (falling edge)
  }
}

void green1() {
  if (digitalRead(0)) {  //Checks the encoder pin
    stepLeft++;          // Increments the stepLeft counter if the pulse goes from HIGH to LOW (rising edge)
  } else {
    stepLeft--;  // Decrements the stepLeft counter if the pulse goes from LOW to HIGH (falling edge)
  }
}

void white2() {
  if (digitalRead(37)) {  // Checks the encoder 2 pin
    stepRight++;          // Increments the stepRight counter if the pulse goes from HIGH to LOW (rising edge)
  } else {
    stepRight--;  // Decrements the stepRight counter if the pulse goes from LOW to HIGH (falling edge)
  }
}

void green2() {
  if (digitalRead(36)) {  // Checks if the encoder 2 pin
    stepRight--;          // Decrements the stepRight counter if the pulse goes from HIGH to LOW (rising edge)
  } else {
    stepRight++;  // Increments the stepRight counter if the pulse goes from LOW to HIGH (falling edge)
  }
}
