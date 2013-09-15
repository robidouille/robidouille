/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php
*/

#ifndef __Sweep__
#define __Sweep__

#include "Arduino.h"

class Sweep {
public:
	Sweep() {}

	void Initialize(int minValue, int maxValue, int start) {
		fMin = minValue;
		fMax = maxValue;
		fCur = start;
	}

	int GetNext(int direction) {
		fCur += direction;
		if (fCur > fMax) {
			fCur = fMax;
		}
		else if (fCur < fMin) {
			fCur = fMin;
		}
		return fCur;
	}

	int Get() const {return fCur;}
	void Set(int value) {fCur = value;}

protected:
	int fCur;
	int fMin;
	int fMax;
};

class TwoWaySweep : public Sweep {
public:
	TwoWaySweep() {}

	void Initialize(int minValue, int maxValue, int start, int dir) {
		Sweep::Initialize(minValue, maxValue, start);
		fDirection = dir;
	}

	int GetNext() {
		fCur += fDirection;
		if (fCur > fMax) {
			fCur = fMax;
		}
		else if (fCur < fMin) {
			fCur = fMin;
		}
		return fCur;
	}

	boolean IsMaxed() const {
		int nextVal = fCur + fDirection;
		return (fCur == fMax && nextVal > fMax) || (fCur == fMin && nextVal < fMin);
	}

	int GetDirection() const {return fDirection;}
	void SetDirection(int value) {fDirection = value;}

protected:
	int fDirection;
};

class AutoReverseTwoWaySweep : public TwoWaySweep {
public:
	AutoReverseTwoWaySweep() {}

	int GetNext() {
		fCur += fDirection;
		if (fCur > fMax) {
			fDirection = -fDirection;
			fCur = fMax;
		}
		else if (fCur < fMin) {
			fDirection = -fDirection;
			fCur = fMin;
		}
		return fCur;
	}
};

#endif
