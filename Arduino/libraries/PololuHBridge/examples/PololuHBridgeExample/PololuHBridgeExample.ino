/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php
*/

#include <HalfHBridge.h>
#include <PololuHBridge.h>

PololuHBridge gMotor;

static void SetupPrescaler() {
  //clear timer 1 bits
  TCCR1B &= ~(0x7);
  //set timer 1 prescaler to 1
  TCCR1B |= 0x1;

  //clear timer 2 bits
  TCCR2B &= ~(0x7);
  //set timer 2 prescaler to 1
  TCCR2B |= 0x1;
}

void setup() {
  SetupPrescaler();
  gMotor.Initialize(2, 6, 4, 5, 3);
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    int cc = Serial.read();
    switch (cc) {
    case '0':
      gMotor.FreeHighPin();
      break;
    case '5':
      gMotor.Stop();
      break;
    case '8':
      gMotor.SetDirection(0);
      gMotor.Start(255);
      break;
    case '2':
      gMotor.SetDirection(1);
      gMotor.Start(255);
      break;
    case '7':
      gMotor.SetDirection(0);
      gMotor.Start(127);
      break;
    case '1':
      gMotor.SetDirection(1);
      gMotor.Start(127);
      break;
    }
  }
}
