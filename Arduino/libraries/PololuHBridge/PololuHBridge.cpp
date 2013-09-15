/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php
*/

#include "Arduino.h"
#include "PololuHBridge.h"

void PololuHBridge::Initialize(int pin1, int enablePin1, int pin2, int enablePin2, int pwmPin) {
	fControl[0].Initialize(pin1, enablePin1);
	fControl[1].Initialize(pin2, enablePin2);

	fPwmPin = pwmPin;
	pinMode(fPwmPin, OUTPUT);

	FreeBothPins();
}
