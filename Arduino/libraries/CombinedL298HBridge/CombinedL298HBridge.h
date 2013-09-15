/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php
*/

#ifndef __CombinedL298HBridge__
#define __CombinedL298HBridge__

#include "Arduino.h"
#include <HalfHBridge.h>

class CombinedL298HBridge {
public:
	CombinedL298HBridge() {
		fDirection = 0;
	}

	void Initialize(int pin1, int enablePin1, int pin2, int enablePin2);

	void SetDirection(int direction) {
		fDirection = direction;
	}

	void Start(int pwm) {
		fControl[1-fDirection].SetDD(LOW, HIGH);
		SetPwm(pwm);
	}

	void Start() {
		fControl[1-fDirection].SetDD(LOW, HIGH);
		SetHigh();
	}

	void Stop() {
		fControl[0].SetDD(LOW, HIGH);
		fControl[1].SetDD(LOW, HIGH);
	}

	void SetPwm(int pwm) {
		fControl[fDirection].SetDA(HIGH, pwm);
	}

	void SetHigh() {
		fControl[fDirection].SetDD(HIGH, HIGH);
	}

	void FreeBothPins() {
		fControl[0].SetEnableD(LOW);
		fControl[1].SetEnableD(LOW);
	}

	void FreeHighPin() {
		fControl[fDirection].SetEnableD(LOW);
	}

private:
	HalfHBridge fControl[2];
	int fDirection;
};

#endif
