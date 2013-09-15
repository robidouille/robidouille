/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php

 Library to use a single pin Ultra Sound sensor.
 Tested with the Devantech Ultrasonic Range Finder SRF05 : http://www.robot-electronics.co.uk/htm/srf05tech.htm
*/

#ifndef __UltraSound1__
#define __UltraSound1__

#include "Arduino.h"

#define kMinMeasureInterval 32

class UltraSound1 {
public:
	UltraSound1() {
		fLastMeasureFinish = millis() - kMinMeasureInterval;
		fLastMeasure = 0;
	}

	void Initialize(int pin) {
		fPin = pin;
		pinMode(fPin, OUTPUT);
		digitalWrite(fPin, LOW);
	}

	int SafeMillisToMeasure() {
		unsigned long curTime = millis();
		if (curTime - fLastMeasureFinish < kMinMeasureInterval) {
			return kMinMeasureInterval - (curTime - fLastMeasureFinish);
		}
		return 0;
	}

	int SafeWaitMeasureMicroseconds() {
		int safeMillisToMeasure = SafeMillisToMeasure();
		if (safeMillisToMeasure > 0) {
			delay(safeMillisToMeasure);
		}
		return MeasureMicroseconds();
	}

	int SafeSkipMeasureMicroseconds() {
		int safeMillisToMeasure = SafeMillisToMeasure();
		if (safeMillisToMeasure > 0) {
			return fLastMeasure;
		}
		return MeasureMicroseconds();
	}

	int MeasureMicroseconds() {
		pinMode(fPin, OUTPUT);
		digitalWrite(fPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(fPin, LOW);

		pinMode(fPin, INPUT);
		fLastMeasure = (int)pulseIn(fPin, HIGH, 32768);

		fLastMeasureFinish = millis();
		return fLastMeasure;
	}

	int MesaureCM() {
		return MeasureMicroseconds() / 58;
	}

private:
	int fPin;
	unsigned long fLastMeasureFinish;
	int fLastMeasure;
};

#endif
