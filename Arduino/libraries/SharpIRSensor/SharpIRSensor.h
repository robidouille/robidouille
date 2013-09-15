/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php

*/

#ifndef __SharpIRSensor__
#define __SharpIRSensor__

#include "Arduino.h"


#define kA 1
#define kB 5
#define kC (-20)
#define kK 9500

#define kIRInfinityThreshold 100

class SharpIRSensor {
public:
	SharpIRSensor() {
		fLastValue = 0;
		fLastValueTime = 0;
	}

	void Initialize(int pinNumber) {
		fPinNumber = pinNumber;
	}

	int GetDistance() {
		int value = analogRead(fPinNumber);
		unsigned long curTime = millis();
		if (fLastValue < kIRInfinityThreshold) {
			if (curTime - fLastValueTime < 40) {
				if (value < kIRInfinityThreshold) {
					fLastValue = value;
					fLastValueTime = curTime;
				}
				return kIRInfinityThreshold;
			}
		}

		fLastValue = value;
		fLastValueTime = curTime;
		if (value < kIRInfinityThreshold) {
			return kIRInfinityThreshold;
		}
		return value;
	}

	static int LinearizeIRSensor(int value) {
		return (kK - kB * (value - kC)) / (kA * (value - kC));
	}

private:
	static unsigned long gLastIRMeasureTime;
	int fPinNumber;
	int fLastValue;
	unsigned long fLastValueTime;
};

#endif
