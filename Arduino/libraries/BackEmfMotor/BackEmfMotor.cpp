/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php
*/

#include "Arduino.h"
#include "BackEmfMotor.h"

//#define __DEBUG_BackEmfMotor__

#define kPwmCycleMicros 10000
#define kMeasureDelayMicros 10
#define kMeasureDurationMicros 100
#define kMaxPwmMicros (kPwmCycleMicros - kMeasureDelayMicros - kMeasureDurationMicros)

#define ABS(x) ((x<0) ? (-x) : (x))

BackEmfMotor::BackEmfMotor() {
	fCommand = kStop;
	fTargetSpeed = 255;
	fPwmMicros = 0;
	fLastStateChangeMicros = 0;
	fState = kStopped;
	fPosition = 0;
	fSpeed = 0;
	fAcceleration = 0;
	fPrevSpeed = 0;
	fIntegral = 0;
	fPrevError = 0;
}

#ifdef __Use_CombinedL298HBridge__
void BackEmfMotor::Initialize(int pin1, int enablePin1, int pin2, int enablePin2, int analogPin) {
	fHBridge.Initialize(pin1, enablePin1, pin2, enablePin2);
#else
void BackEmfMotor::Initialize(int pin1, int enablePin1, int pin2, int enablePin2, int pwmPin, int analogPin) {
	fHBridge.Initialize(pin1, enablePin1, pin2, enablePin2, pwmPin);
#endif

	fAnalogPin = analogPin;

	fHBridge.Stop();
}

bool BackEmfMotor::Service() {
	unsigned long elapsedMicros = micros() - fLastStateChangeMicros;
	switch (fState) {
	case kStarted:
		if (elapsedMicros > ABS(fPwmMicros)) {
			WaitToMeasure();
		}
		break;
	case kWaitToMeasure:
		if (elapsedMicros > kMeasureDelayMicros) {
			StartMeasure();
		}
		break;
	case kMeasuring: {
		fMeasure.Add(analogRead(fAnalogPin));

		if (elapsedMicros > (kPwmCycleMicros - ABS(fPwmMicros))) {
			UpdatePwm();
			Start();
			return true;
		}
	} break;
	case kFreed:
	case kStopped:
		break;
	}
	return false;
}

#ifdef __DEBUG_BackEmfMotor__
static void DebugPrintVar(char * name, int num) {
	Serial.print(name);
	Serial.print(num);
	Serial.print("=");
}

static void DebugPrint(char * name, int num, int value) {
	DebugPrintVar(name, num);
	Serial.print(value);
}

static void DebugPrint(char * name, int num, unsigned long value) {
	DebugPrintVar(name, num);
	Serial.print(value);
}
#endif

void BackEmfMotor::UpdatePwm() {
	// Measure what actually happened
	int averageMeasure = fMeasure.GetAverage();
	int measure = fMeasure.GetMax();

	if (fPwmMicros < 0) {
		fSpeed = -measure;
		unsigned long newPosition = fPosition + fSpeed;
		if (newPosition <= fPosition) { // check for overflow
			fPosition = newPosition;
		}
		else {
			fPosition = 0;
		}
	}
	else {
		fSpeed = measure;
		unsigned long newPosition = fPosition + fSpeed;
		if (newPosition >= fPosition) { // check for overflow
			fPosition = newPosition;
		}
		else {
			fPosition = 0xFFFFffff;
		}
	}

	fAcceleration = fSpeed - fPrevSpeed;
	fPrevSpeed = fSpeed;

	// PID algorithm.
	int error = fTargetSpeed - fSpeed;

	fIntegral += error;
	if (fIntegral > 5000) {
		fIntegral = 5000;
	}
	else if (fIntegral < -5000) {
		fIntegral = -5000;
	}

	int derivative = error - fPrevError;
	fPrevError = error;

	if (fTargetSpeed != kMaxInt) {
		fPwmMicros += error / 4;
		//fPwmMicros += fIntegral / 4;
		//fPwmMicros += derivative / 4;
	}

	if (fPwmMicros < -kMaxPwmMicros) fPwmMicros = -kMaxPwmMicros;
	else if (fPwmMicros > kMaxPwmMicros) fPwmMicros = kMaxPwmMicros;

#ifdef __DEBUG_BackEmfMotor__
	DebugPrint("mt", fAnalogPin, fMeasure.GetAccumulator());
	DebugPrint(" mc", fAnalogPin, fMeasure.GetCount());
	DebugPrint(" min", fAnalogPin, fMeasure.GetMin());
	DebugPrint(" avg", fAnalogPin, averageMeasure);
	DebugPrint(" max", fAnalogPin, fMeasure.GetMax());
	DebugPrint(" tgt", fAnalogPin, fTargetSpeed);
	DebugPrint(" tgt", fAnalogPin, fTargetSpeed);
	DebugPrint(" pwm", fAnalogPin, fPwmMicros);
	DebugPrint(" err", fAnalogPin, error);
	DebugPrint(" int", fAnalogPin, fIntegral);
	DebugPrint(" der", fAnalogPin, derivative);
	DebugPrint(" min", fAnalogPin, fMinMeasure);
	DebugPrint(" pos", fAnalogPin, fPosition);
	DebugPrint(" spd", fAnalogPin, fSpeed);
	DebugPrint(" acc", fAnalogPin, fAcceleration);
	Serial.println("");
#endif
}
