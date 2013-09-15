/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php
*/

#include "Arduino.h"
#include "BackEmfMotor.h"

#define kPwmCycleMicros 10000
#define kMeasureDelayMicros 10
#define kMeasureDurationMicros 100
#define kMaxPwmMicros (kPwmCycleMicros - kMeasureDelayMicros - kMeasureDurationMicros)

BackEmfMotor::BackEmfMotor() {
	fCommand = kStop;
	fTargetSpeed = 255;
	fPwmMicros = 1;
	fLastStateChangeMicros = 0;
	fState = kStopped;
	fMeasureAccumulator = 0;
	fMeasureCount = 0;
	fMaxMeasure = 0;
	fMinMeasure = 32767;
	fCurrentSpeed = 0;
	fIntegral = 0;
	fPrevError = 0;
#ifdef __DEBUG_BackEmfMotor__
	fDebugPin = -1;
#endif
}

#ifdef __Use_CombinedL298HBridge__
void BackEmfMotor::Initialize(int pin1, int enablePin1, int pin2, int enablePin2, int analogPin) {
	fHBridge.Initialize(pin1, enablePin1, pin2, enablePin2);
#else
void BackEmfMotor::Initialize(int pin1, int enablePin1, int pin2, int enablePin2, int pwmPin, int analogPin) {
	fHBridge.Initialize(pin1, enablePin1, pin2, enablePin2, pwmPin);
#endif

#ifdef __DEBUG_BackEmfMotor__
	if (fDebugPin >= 0) {
		pinMode(fDebugPin, OUTPUT);
		digitalWrite(fDebugPin, false);
	}
#endif

	fAnalogPin = analogPin;

	fHBridge.Stop();
}

void BackEmfMotor::Service() {
	unsigned long elapsedMicros = micros() - fLastStateChangeMicros;
	switch (fState) {
	case kStarted:
		if (elapsedMicros > fPwmMicros) {
#ifdef __DEBUG_BackEmfMotor__
			if (fDebugPin >= 0) {
				digitalWrite(fDebugPin, true);
			}
#endif
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

		if (elapsedMicros > (kPwmCycleMicros - fPwmMicros)) {
			if (fTargetSpeed >= 0) {
				UpdatePwm();

				fMaxMeasure = 0;
				fMinMeasure = 32767;
			}
			Start();
#ifdef __DEBUG_BackEmfMotor__
			if (fDebugPin >= 0) {
				digitalWrite(fDebugPin, false);
			}
#endif
		}
	} break;
	case kFreed:
	case kStopped:
		break;
	}
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

	fPwmMicros += error / 4;
	//fPwmMicros += fIntegral / 4;
	//fPwmMicros += derivative / 4;

	if (fPwmMicros < 1) fPwmMicros = 1;
	else if (fPwmMicros > kMaxPwmMicros) fPwmMicros = kMaxPwmMicros;

#ifdef __DEBUG_BackEmfMotor__
	Serial.print("m=");
	Serial.print(fMeasureAccumulator);
	Serial.print(" mc=");
	Serial.print(fMeasureCount);
	Serial.print(" min=");
	Serial.print(fMinMeasure);
	Serial.print(" avg=");
	Serial.print(averageMeasure);
	Serial.print(" max=");
	Serial.print(fMaxMeasure);
	Serial.print(" t=");
	Serial.print(fTargetSpeed);
	Serial.print(" pwm=");
	Serial.print(fPwmMicros);
	Serial.print(" e=");
	Serial.print(error);
	Serial.print(" i=");
	Serial.print(fIntegral);
	Serial.println("");
#endif
}
