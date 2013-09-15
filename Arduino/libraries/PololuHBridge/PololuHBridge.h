/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php
*/

#ifndef __PololuHBridge__
#define __PololuHBridge__

#include "Arduino.h"
#include <HalfHBridge.h>

class PololuHBridge {
public:
	PololuHBridge() {
		fDirection = 0;
	}

	void Initialize(int pin1, int enablePin1, int pin2, int enablePin2, int pwmPin);

	void SetDirection(int direction) {
		fDirection = direction;
	}

	void Start(int pwm) {
		fControl[fDirection].SetDD(HIGH, HIGH);
		fControl[1-fDirection].SetDD(LOW, HIGH);
		SetPwm(pwm);
	}

	void Start() {
		fControl[fDirection].SetDD(HIGH, HIGH);
		fControl[1-fDirection].SetDD(LOW, HIGH);
		SetHigh();
	}

	void Stop() {
		fControl[0].SetDD(LOW, HIGH);
		fControl[1].SetDD(LOW, HIGH);
	}

	void SetPwm(int pwm) {
		analogWrite(fPwmPin, pwm);
	}

	void SetHigh() {
		digitalWrite(fPwmPin, HIGH);
	}

	void FreeBothPins() {
		fControl[0].SetEnableD(LOW);
		fControl[1].SetEnableD(LOW);
		digitalWrite(fPwmPin, LOW);
	}

	void FreeHighPin() {
		fControl[fDirection].SetEnableD(LOW);
		digitalWrite(fPwmPin, LOW);
	}

private:
	HalfHBridge fControl[2];
	int fPwmPin;
	int fDirection;
};

#endif
