/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php

*/

#ifndef __InterpolatedServo__
#define __InterpolatedServo__

#include "Arduino.h"
#include <Debug.h>

class InterpolatedServo {
public:
	InterpolatedServo() {
	}

	void Initialize(int pinNumber, int angle0, int angle90, int startValue) {
		fServo.attach(pinNumber);
		fAngle0 = angle0;
		fAngle90 = angle90;
		SetValue(startValue);
	}

	void SetValue(int angle) {
		int value = ( (90 - angle) * fAngle0 + angle * fAngle90 ) / 90;
		//DEBUG3(angle, " -> ", value);
		fServo.write(value);
	}

	void SetRaw(int value) {
		fServo.write(value);
	}

private:
	Servo fServo;
	int fAngle0;
	int fAngle90;
};

class SpeedLimitedServo : public InterpolatedServo {
public:
	SpeedLimitedServo() {
	}

	void Initialize(int pinNumber, int angle0, int angle90, int speed, int startValue) {
		InterpolatedServo::Initialize(pinNumber, angle0, angle90, startValue);
		fSpeed = speed;
		fCurValue = startValue;
		fLastService = millis();
	}

	void SetValue(int angle) {
		if (fSetValue != angle) {
			Service();
			fSetValue = angle;
		}
		Service();
	}

	void Service() {
		unsigned long curTime = millis();

		int direction = fSetValue - fCurValue;
		if (direction == 0) {
			fLastService = curTime;
			//DEBUG3(millis(), ": reached ", fSetValue);
			return;
		}

		unsigned long elapsed = curTime - fLastService;
		int setValue;
		if (direction < 0) {
			setValue = fCurValue - ((elapsed * fSpeed) / 1000);
			//DEBUG_(" (-");
			//DEBUG_(setValue);
			//DEBUG_(")");
			if (setValue < fSetValue) {
				setValue = fSetValue;
			}
		}
		else {
			setValue = fCurValue + ((elapsed * fSpeed) / 1000);
			//DEBUG_(" (+");
			//DEBUG_(setValue);
			//DEBUG_(")");
			if (setValue > fSetValue) {
				setValue = fSetValue;
			}
		}

		if (fCurValue != setValue) {
			fCurValue = setValue;
			fLastService = curTime;
			InterpolatedServo::SetValue(setValue);
		}

		//DEBUG3(curTime, ": ", setValue);
	}

	int GetCurValue() const {return fCurValue;}
	int GetTargetValue() const {return fSetValue;}
	bool IsIdle() const {return fCurValue == fSetValue;}
	void Stop() {SetValue(fCurValue);}

private:
	int fSpeed;
	int fSetValue;
	int fCurValue;
	unsigned long fLastService;
};

#endif
