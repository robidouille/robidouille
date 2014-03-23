/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php

 Too see this code in action:
   https://www.youtube.com/watch?v=QgYhapXIgk0
*/

#include <HalfHBridge.h>
#include <CombinedL298HBridge.h>
#include <BackEmfMotor.h>
#include <CommandStream.h>

#define kMotorCount 2

CommandStream gCommand;
BackEmfMotor gMotor[kMotorCount];

static void StartTargetSpeed(int speed, int idx) {
  gMotor[idx].SetTargetSpeed(speed);
  gMotor[idx].SetCommand(BackEmfMotor::kStart);
  gMotor[idx].Commit();
}

static void StartPwmMicros(int pwmMicros, int idx) {
  gMotor[idx].SetPwmMicros(pwmMicros);
  gMotor[idx].SetCommand(BackEmfMotor::kStart);
  gMotor[idx].Commit();
}

static void FreeMotor(int idx) {
  gMotor[idx].SetCommand(BackEmfMotor::kFree);
  gMotor[idx].Commit();
}

static void StopMotor(int idx) {
  gMotor[idx].SetCommand(BackEmfMotor::kStop);
  gMotor[idx].Commit();
}

inline int Abs(int value) {
  return value > 0 ? value : (-value);
}

void setup() {
  gMotor[0].Initialize(2, 3, 4, 5, 0);
  gMotor[1].Initialize(8, 9, 10, 11, 1);

  Serial.begin(115200);
}

int gSpeed = -1;
int gPwm = -1;
int gDirection = 0;
int gCurrentMotor = 0;

static void StartCurrentMotor() {
  if (gSpeed >= 0) {
    StartTargetSpeed(gDirection == 0 ? gSpeed : (-gSpeed), gCurrentMotor);
  }
  else if (gPwm >= 0) {
    StartPwmMicros(gDirection == 0 ? gPwm : (-gPwm), gCurrentMotor);
  }
  else {
    FreeMotor(gCurrentMotor);
  }
}

static void ZeroCurrentMotor() {
  if (gSpeed >= 0) {
    StartTargetSpeed(0, gCurrentMotor);
  }
  else if (gPwm >= 0) {
    StartPwmMicros(0, gCurrentMotor);
  }
  else {
    FreeMotor(gCurrentMotor);
  }
}

void CheckSerial() {
  if (Serial.available() == 0) {
    return;    
  }

  gCommand.HandleChar(Serial.read());
  if (!gCommand.HasCommand()) {
    return;
  }

  int value = gCommand.GetValue();
  switch (gCommand.GetCommand()) {
  case 'f':
    gSpeed = -1;
    gPwm = -1;
    for (int ii = 0 ; ii < kMotorCount ; ii++) {
      gMotor[ii].SetCommand(BackEmfMotor::kFree);
      gMotor[ii].Commit();
    }
    Serial.println("free");
    break;
  case 'b':
    gSpeed = -1;
    gPwm = -1;
    for (int ii = 0 ; ii < kMotorCount ; ii++) {
      gMotor[ii].SetCommand(BackEmfMotor::kStop);
      gMotor[ii].Commit();
    }
    Serial.println("break");
    break;
  case 'd':
    if (value == 0 || value == 1) {
      gDirection = value;
      StartCurrentMotor();
      Serial.print("direction ");
      Serial.println(gDirection);
    }
    break;
  case 'm':
    if (value >= 0 && value < kMotorCount) {
      gCurrentMotor = value;
      Serial.print("motor ");
      Serial.println(gCurrentMotor);
    }
    break;
  case 'l':
    gPwm = 8000;
    gSpeed = -1;

    gMotor[0].SetPosition(0);
    gMotor[1].SetPosition(0);

    gCurrentMotor = 1;
    gDirection = 1;
    ZeroCurrentMotor();

    gCurrentMotor = 0;
    gDirection = 0;
    StartCurrentMotor();

    Serial.print("Limit Mode");
    break;
  case 's':
    gSpeed = value;
    gPwm = -1;
    StartCurrentMotor();
    Serial.print("speed ");
    Serial.println(gSpeed);
    break;
  case 'p':
    gPwm = value * 1000;
    gSpeed = -1;
    StartCurrentMotor();
    Serial.print("pwm micros ");
    Serial.println(gPwm);
    break;
  }
}

void UpdateCurrentMotor() {
  static unsigned long gLastCourseChangeMicros = 0;
  unsigned long nowMicros = micros();
  
  BackEmfMotor & motor(gMotor[gCurrentMotor]);

  int resistance = Abs(motor.GetPwmMicros()) / (Abs(motor.GetSpeed()) + 1);
  if (resistance > 150 && (nowMicros - gLastCourseChangeMicros > 2000L * 1000L)) {
    ZeroCurrentMotor();

    gCurrentMotor++;
    if (gCurrentMotor > 1) {
      gCurrentMotor = 0;
      gDirection = 1 - gDirection;
    }

#if 0
    Serial.print("m=");
    Serial.print(gCurrentMotor);
    Serial.print(" d=");
    Serial.print(gDirection);
    Serial.println("");
#endif

    if (gDirection == 0) {
      gMotor[gCurrentMotor].SetPosition(0);
    }

    StartCurrentMotor();
    gLastCourseChangeMicros = nowMicros;
  }
#if 0  
  Serial.print("pwm=");
  Serial.print(motor.GetPwmMicros());
  Serial.print(" spd=");
  Serial.print(motor.GetCurrentSpeed());
  Serial.print(" res=");
  Serial.print(resistance);
  Serial.print(" t=");
  Serial.print((nowMicros - gLastCourseChangeMicros) / 1000L);
  Serial.println("");
#endif
}

void loop() {
  CheckSerial();

  bool updated = false;
  for (int ii = 0 ; ii < kMotorCount ; ii++) {
    if (gMotor[ii].Service() && ii == gCurrentMotor) {
      updated = true;
    }
  }
  if (updated) {
    UpdateCurrentMotor();
  }
}

