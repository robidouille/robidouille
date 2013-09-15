/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php
*/

#define __Use_CombinedL298HBridge__

#include <HalfHBridge.h>
#ifdef __Use_CombinedL298HBridge__
#include <CombinedL298HBridge.h>
#else
#include <PololuHBridge.h>
#endif
#include <BackEmfMotor.h>

BackEmfMotor gMotor;

static void StartTargetSpeed(int speed, int dir) {
  gMotor.SetTargetSpeed(speed);
  gMotor.SetCommand(BackEmfMotor::kStart);
  gMotor.SetDirection(dir);
}

static void StartPWMSpeed(int pwm, int dir) {
  gMotor.SetPwm(pwm);
  gMotor.SetCommand(BackEmfMotor::kStart);
  gMotor.SetDirection(dir);
}

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
#ifdef __Use_CombinedL298HBridge__
  gMotor.Initialize(2, 3, 4, 11, 1);
#else
  gMotor.Initialize(2, 6, 4, 5, 3, 0);
#endif

  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    int cc = Serial.read();
    switch (cc) {
    case '0':
      gMotor.SetCommand(BackEmfMotor::kFree);
      break;
    case '5':
      gMotor.SetCommand(BackEmfMotor::kStop);
      break;
    case '8':
      StartTargetSpeed(256, 0);
      break;
    case '2':
      StartTargetSpeed(256, 1);
      break;
    case '7':
      StartPWMSpeed(255, 0);
      break;
    case '1':
      StartPWMSpeed(255, 1);
      break;
    case '9':
      StartPWMSpeed(128, 0);
      break;
    case '3':
      StartPWMSpeed(128, 1);
      break;
    }

    gMotor.Commit();
  }

  gMotor.Service();
}
