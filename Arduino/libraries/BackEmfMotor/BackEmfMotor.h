/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php
*/

#ifndef __BackEmfMotor__
#define __BackEmfMotor__

#define __Use_CombinedL298HBridge__

#include "Arduino.h"

#ifdef __Use_CombinedL298HBridge__
#include <CombinedL298HBridge.h>
#else
#include <PololuHBridge.h>
#endif

#define kMaxInt 32767

class BackEmfMotor {
public:
	enum Command {kStop, kFree, kStart};

	BackEmfMotor();

#ifdef __Use_CombinedL298HBridge__
	void Initialize(int pin1, int enablePin1, int pin2, int enablePin2, int analogPin);
#else
	void Initialize(int pin1, int enablePin1, int pin2, int enablePin2, int pwmPin, int analogPin);
#endif

	void SetCommand(Command command) {fCommand = command;}
	void SetTargetSpeed(int speed) {fTargetSpeed = speed;}
	void SetPwmMicros(int pwmMicros) {
		fTargetSpeed = kMaxInt;
		fPwmMicros = pwmMicros;
	}

	int GetTargetSpeed() const {return fTargetSpeed;}
	int GetPwmMicros() const {return fPwmMicros;}

	unsigned long GetPosition() const {return fPosition;}
	void SetPosition(unsigned long position) {fPosition = position;}

	int GetSpeed() const {return fSpeed;}
	int GetAcceleration() const {return fAcceleration;}

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

	bool Service();

private:
	enum State {kStopped, kFreed, kStarted, kWaitToMeasure, kMeasuring};

	void SetState(State state) {
		fState = state;
		fLastStateChangeMicros = micros();
	}

	void Start() {
		fHBridge.SetDirection((fPwmMicros < 0) ? 1 : 0);
		if (fPwmMicros != 0) {
			fHBridge.Start();
			SetState(kStarted);
		}
		else {
			WaitToMeasure();
		}
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
	State fState;
	unsigned long fLastStateChangeMicros;
	unsigned long fMeasureAccumulator;
	unsigned long fPosition;
	int fSpeed;
	int fAcceleration;
	int fPrevSpeed;

	int fTargetSpeed;
	int fIntegral;
	int fPrevError;
	int fPwmMicros;
	int fMinMeasure;
	int fMaxMeasure;
	int fMeasureCount;
};

#endif
