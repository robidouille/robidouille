/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php
*/

#ifndef __BackEmfMotor__
#define __BackEmfMotor__

#define __DEBUG_BackEmfMotor__

#define __Use_CombinedL298HBridge__

#include "Arduino.h"

#ifdef __Use_CombinedL298HBridge__
#include <CombinedL298HBridge.h>
#else
#include <PololuHBridge.h>
#endif

class BackEmfMotor {
public:
	enum Command {kStop, kFree, kStart};

	BackEmfMotor();

#ifdef __Use_CombinedL298HBridge__
	void Initialize(int pin1, int enablePin1, int pin2, int enablePin2, int analogPin);
#else
	void Initialize(int pin1, int enablePin1, int pin2, int enablePin2, int pwmPin, int analogPin);
#endif

	int GetCurrentSpeed() const {return fCurrentSpeed;}
	void SetCommand(Command command) {fCommand = command;}
	void SetDirection(int direction) {fHBridge.SetDirection(direction);}
	void SetTargetSpeed(int speed) {fTargetSpeed = speed;}
	void SetPwmMicros(int pwmMicros) {
		fTargetSpeed = -1;
		fPwmMicros = pwmMicros;
	}

	int GetTargetSpeed() const {return fTargetSpeed;}
	int GetPwmMicros() const {return fPwmMicros;}

	void Commit() {
		switch (fCommand) {
		case kStart:
			//Serial.println("Start");
			Start();
			break;
		case kStop:
			//Serial.println("Stop");
			Stop();
			break;
		case kFree:
			//Serial.println("Free");
			Free();
			break;
		}
	}

	void Service();

#ifdef __DEBUG_BackEmfMotor__
	int fDebugPin;
#endif

private:
	enum State {kStopped, kFreed, kStarted, kWaitToMeasure, kMeasuring};

	void SetState(State state) {
		fState = state;
		fLastStateChangeMicros = micros();
	}

	void Start() {
		fHBridge.Start();
		SetState(kStarted);
	}

	void Stop() {
		fHBridge.Stop();
		SetState(kStopped);
	}

	void Free() {
		fHBridge.FreeHighPin();
		SetState(kFreed);
	}

	void WaitToMeasure() {
		fHBridge.FreeHighPin();
		SetState(kWaitToMeasure);
	}

	void StartMeasure() {
		fMeasureAccumulator = 0;
		fMeasureCount = 0;
		SetState(kMeasuring);
	}

	void UpdatePwm();

#ifdef __Use_CombinedL298HBridge__
	CombinedL298HBridge fHBridge;
#else
	PololuHBridge fHBridge;
#endif
	int fAnalogPin;

	Command fCommand;
	int fTargetSpeed;
	int fCurrentSpeed;
	int fIntegral;
	int fPrevError;
	int fPwmMicros;
	unsigned long fLastStateChangeMicros;
	State fState;
	unsigned long fMeasureAccumulator;
	int fMinMeasure;
	int fMaxMeasure;
	int fMeasureCount;
};

#endif
