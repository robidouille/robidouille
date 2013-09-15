/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php
*/

#ifndef __HalfHBridge__
#define __HalfHBridge__

#include "Arduino.h"

class HalfHBridge {
public:
	HalfHBridge() {}

	void Initialize(int pin, int enablePin) {
		fPin = pin;
		fEnablePin = enablePin;

		pinMode(fPin, OUTPUT);
		pinMode(fEnablePin, OUTPUT);
	}

	void SetDD(boolean value, boolean enable) {
		digitalWrite(fPin, value);
		digitalWrite(fEnablePin, enable);
	}

	void SetDA(boolean value, int enable) {
		digitalWrite(fPin, value);
		analogWrite(fEnablePin, enable);
	}

	void SetAD(int value, boolean enable) {
		analogWrite(fPin, value);
		digitalWrite(fEnablePin, enable);
	}

	void SetD(boolean enable) {
		digitalWrite(fPin, enable);
	}

	void SetA(int value) {
		analogWrite(fPin, value);
	}

	void SetEnableD(boolean enable) {
		digitalWrite(fEnablePin, enable);
	}

	void SetEnableA(int value) {
		analogWrite(fEnablePin, value);
	}

private:
	int fEnablePin;
	int fPin;
};

#endif
