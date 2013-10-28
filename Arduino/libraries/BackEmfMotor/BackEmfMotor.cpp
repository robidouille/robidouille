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
	fMeasureAccumulator = 0;
	fMeasureCount = 0;
	fMaxMeasure = 0;
	fMinMeasure = 32767;
	fCurrentSpeed = 0;
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
		int measure = analogRead(fAnalogPin);
		fMeasureAccumulator += measure;
		fMeasureCount++;
		if (measure > fMaxMeasure) fMaxMeasure = measure;
		if (measure < fMinMeasure) fMinMeasure = measure;

		if (elapsedMicros > (kPwmCycleMicros - ABS(fPwmMicros))) {
			UpdatePwm();

			fMaxMeasure = 0;
			fMinMeasure = 32767;

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

void BackEmfMotor::UpdatePwm() {
	int averageMeasure = fMeasureAccumulator / fMeasureCount;

	fCurrentSpeed = fMaxMeasure;

	int error = fTargetSpeed - fCurrentSpeed;

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
	Serial.print("mt=");
	Serial.print(fMeasureAccumulator);
	Serial.print(" mc=");
	Serial.print(fMeasureCount);
	Serial.print(" min=");
	Serial.print(fMinMeasure);
	Serial.print(" avg=");
	Serial.print(averageMeasure);
	Serial.print(" max=");
	Serial.print(fMaxMeasure);
	Serial.print(" tgt=");
	Serial.print(fTargetSpeed);
	Serial.print(" pwm=");
	Serial.print(fPwmMicros);
	Serial.print(" err=");
	Serial.print(error);
	Serial.print(" int=");
	Serial.print(fIntegral);
	Serial.print(" der=");
	Serial.print(derivative);
	Serial.println("");
#endif
}
