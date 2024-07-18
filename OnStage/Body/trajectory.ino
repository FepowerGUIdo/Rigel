bool makeBin() {
  if (dt.check() == 1) {
    byte buffer[24];
    //checks if there is "remaining movement to perform" from the data on the SD card file
    if (bin.available()) {
      bin.read(buffer, 24);
      memcpy(&thetaL, &buffer[0], 4);
      memcpy(&thetaLDOt, &buffer[4], 4);
      memcpy(&thetaLDDOt, &buffer[8], 4);
      memcpy(&thetaR, &buffer[12], 4);
      memcpy(&thetaRDOt, &buffer[16], 4);
      memcpy(&thetaRDDOt, &buffer[20], 4);

    }
    //if there is no remaining movement to perform, all the parameters are set to zero, thus, the movement stops
    else {
      thetaLDDOt = 0;
      thetaLDOt = 0;
      thetaRDDOt = 0;
      thetaRDOt = 0;
      timePass++;
    }
    //Serial.println(micros() - teste);
    float thetaRSens = (stepRight * 2 * PI) / 2400;
    float thetaLSens = (stepLeft * 2 * PI) / 2400;
    //formula to calculate the speed of the motor based on the data generated from the trajectory generator
    float speedL = lastSpeedL / (kd * tau + 1) + tau * (thetaLDDOt + kd * thetaLDOt - kp * (thetaLSens - thetaL)) / (kd * tau + 1);
    float speedR = lastSpeedR / (kd * tau + 1) + tau * (thetaRDDOt + kd * thetaRDOt - kp * (thetaRSens - thetaR)) / (kd * tau + 1);
    //sets the speed just calculated above as the last speed calculated, so that on the next iteration the speed found on the most-recent one is used as the last speed
    lastSpeedL = speedL;
    lastSpeedR = speedR;

    //calculates the real speed of the robot knowing the values for the reductions delcared above and knowing that the wheel is a circle
    float realSpeedL = (speedL * 800 ) / (2 * PI);
    float realSpeedR = (speedR * 800) / (2 * PI);


    if (timePass >= 500) {
      realSpeedL = 0;
      realSpeedR = 0;
      return false;
    }
    //sets the speed of the motors as their "realSpeed"
    stepperL.setSpeed(realSpeedL);
    stepperR.setSpeed(realSpeedR);

    Serial.print("RSens:"); Serial.println(thetaRSens);
    Serial.print("LSens:"); Serial.println(thetaLSens);
    /*  Serial.print("ThetaLDot:"); Serial.println(thetaLDOt);
    Serial.print("speedL:"); Serial.println(speedL);*/
    Serial.print("R:"); Serial.println(thetaR);
    Serial.print("L:"); Serial.println(thetaL);
    //Serial.println(thetaLSens);
  }

  //makes both motors start running at the respective speed calculated by the trajectory generator parameters, which are used to set the values of distance, velocity and acceleration shown above
  stepperL.runSpeed();
  stepperR.runSpeed();
  return true;
}