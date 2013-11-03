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

static void StartTargetSpeed(int speed) {
  gMotor.SetTargetSpeed(speed);
  gMotor.SetCommand(BackEmfMotor::kStart);
}

static void StartPwmMicros(int pwmMicros) {
  gMotor.SetPwmMicros(pwmMicros);
  gMotor.SetCommand(BackEmfMotor::kStart);
}

void setup() {
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
      StartTargetSpeed(256);
      break;
    case '2':
      StartTargetSpeed(-256);
      break;
    case '7':
      StartPwmMicros(9000);
      break;
    case '1':
      StartPwmMicros(-9000);
      break;
    case '9':
      StartPwmMicros(6000);
      break;
    case '3':
      StartPwmMicros(-6000);
      break;
    }

    gMotor.Commit();
  }

  gMotor.Service();
}
