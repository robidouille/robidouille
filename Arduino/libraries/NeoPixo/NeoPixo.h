/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php
*/

#ifndef __NeoPixo__
#define __NeoPixo__

#include "Arduino.h"
#include "Adafruit_NeoPixel.h"

class NeoPixo {
public:
	NeoPixo(Adafruit_NeoPixel & pixels, uint32_t * colors, uint8_t colorCount);

	void Off();
	void RandomSparks(uint8_t count);
	void Spinny(uint8_t count, uint8_t totalPowerOf2, uint32_t delayMs, int speed);
	void Rainbow(uint16_t multiplier, int speed);
	void RainbowCycle(uint16_t multiplier, int speed);
	void Race(uint8_t count, int speed);
	void Lightning();
	void Countdown(uint8_t count, int speed, uint32_t delayMs);
	void Fireworks(uint32_t delayMs);
private:
	void SetStrip(uint32_t color);
	uint32_t Wheel(byte wheelPos);

	Adafruit_NeoPixel & fPixels;
	uint32_t * fColors;
	uint8_t fColorCount;
	uint8_t fOffset;
};

#endif
