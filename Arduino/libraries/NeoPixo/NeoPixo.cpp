/*

 Copyright (c) by Emil Valkov,
 All rights reserved.

 License: http://www.opensource.org/licenses/bsd-license.php

 Rainbow algorithm from the Adafruit NeoPixel library strandtest example:
   http://github.com/adafruit/Adafruit_NeoPixel
*/

#include "Arduino.h"
#include "NeoPixo.h"

NeoPixo::NeoPixo(Adafruit_NeoPixel & pixels, uint32_t * colors, uint8_t colorCount) : fPixels(pixels) {
	fColors = colors;
	fColorCount = colorCount;
	fOffset = 0;
}

void NeoPixo::Off() {
	SetStrip(0);
	fPixels.show();
}

void NeoPixo::RandomSparks(uint8_t count) {
	SetStrip(0);
	for (uint8_t ii = 0 ; ii < count ; ii++) {
		fPixels.setPixelColor(random(fPixels.numPixels()), fColors[random(fColorCount)]);
	}
	fPixels.show();
	delay(10);
}

void NeoPixo::Spinny(uint8_t count, uint8_t totalPowerOf2, uint32_t delayMs, int speed) {
	for (uint16_t ii = 0 ; ii < fPixels.numPixels() ; ii++) {
		uint32_t color = 0;
		uint16_t offsetI = fOffset + ii;
		if ((offsetI & ((1 << totalPowerOf2) - 1)) < count) {
			color = fColors[(offsetI & (1 << totalPowerOf2)) >> totalPowerOf2];
		}
		fPixels.setPixelColor(ii, color);
	}
	fPixels.show();
	fOffset += speed;
	delay(delayMs);
}

void NeoPixo::Rainbow(uint16_t multiplier, int speed) {
	for (uint16_t ii = 0 ; ii < fPixels.numPixels() ; ii++) {
		fPixels.setPixelColor(ii, Wheel((ii * multiplier + fOffset) & 255));
	}
	fPixels.show();
	fOffset += speed;
}

// Slightly different, this makes the rainbow equally distributed throughout
void NeoPixo::RainbowCycle(uint16_t multiplier, int speed) {
	for (uint16_t ii = 0 ; ii < fPixels.numPixels() ; ii++) {
		fPixels.setPixelColor(ii, Wheel(((ii * multiplier * 256 / fPixels.numPixels()) + fOffset) & 255));
	}
	fPixels.show();
	fOffset += speed;
}

// Input a value 0 to 255 to get a color value.
// The colors are a transition r - g - b - back to r.
uint32_t NeoPixo::Wheel(byte wheelPos) {
	if (wheelPos < 85) {
		return fPixels.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
	}
	else if (wheelPos < 170) {
		wheelPos -= 85;
		return fPixels.Color(255 - wheelPos * 3, 0, wheelPos * 3);
	}
	else {
		wheelPos -= 170;
		return fPixels.Color(0, wheelPos * 3, 255 - wheelPos * 3);
	}
}

#define kSpriteCount 10
struct Sprite {
	int fPos;
	uint8_t fCount;
	uint8_t fSpeed;
	uint32_t fStartTime;
};

Sprite gSprites[kSpriteCount];

void NeoPixo::Race(uint8_t count, int speed) {
	static uint32_t gLastCall = 0;
	if (count > kSpriteCount) count = kSpriteCount;
	uint32_t now = millis();

	if (gLastCall == 0) {
		for (uint8_t ii = 0 ; ii < count ; ii++) {
			gSprites[ii].fPos = random(fPixels.numPixels());
			gSprites[ii].fSpeed = random(10);
			gSprites[ii].fCount = gSprites[ii].fSpeed;
		}
		gLastCall = now;
	}

	if (now - gLastCall > 1000) {
		for (uint8_t ii = 0 ; ii < count ; ii++) {
			gSprites[ii].fSpeed = random(10);
		}
		gLastCall = now;
	}

	SetStrip(0);
	for (uint8_t ii = 0 ; ii < count ; ii++) {
		if (gSprites[ii].fCount == 0) {
			gSprites[ii].fCount = gSprites[ii].fSpeed;
			gSprites[ii].fPos += speed;
			if (gSprites[ii].fPos < 0) {
				gSprites[ii].fPos = fPixels.numPixels() - 1;
			}
			else if (gSprites[ii].fPos >= fPixels.numPixels()) {
				gSprites[ii].fPos = 0;
			}
		}
		else {
			gSprites[ii].fCount--;
		}
		fPixels.setPixelColor(gSprites[ii].fPos, fColors[ii % fColorCount]);
	}
	fPixels.show();
}

void NeoPixo::Lightning() {
	if (fOffset < 10) {
		SetStrip(0xffffff);
		fPixels.show();
		delay(10);
		SetStrip(0);
		fPixels.show();
		delay(20 + random(200));
	}
	else {
		delay(30);
	}
	fOffset++;
}

#define kPixelsPerSprite 7
void NeoPixo::Countdown(uint8_t count, int speed, uint32_t delayMs) {
	static bool gInit = false;

	uint8_t center = fPixels.numPixels() / 2;

	if (!gInit) {
		for (uint8_t ii = 0 ; ii < kSpriteCount ; ii++) {
			gSprites[ii].fPos = ii * kPixelsPerSprite;
		}
		gInit = true;
	}

	SetStrip(0);
	for (uint8_t ii = 0 ; ii < kSpriteCount ; ii++) {
		uint32_t color = fColors[ii % fColorCount];
		for (uint8_t jj = 0 ; jj < kPixelsPerSprite ; jj++) {
			int idx1 = center + gSprites[ii].fPos + jj;
			if (idx1 < fPixels.numPixels()) fPixels.setPixelColor(idx1, color);

			int idx2 = center - gSprites[ii].fPos - jj;
			if (idx2 >= 0) fPixels.setPixelColor(idx2, color);
		}
	}
	fPixels.show();

	if (gSprites[count].fPos <= center) {
		gSprites[count].fPos += speed;
	}
	delay(delayMs);
}

#define kFireworkSize 5
void NeoPixo::Fireworks(uint32_t delayMs) {
	static bool gInit = false;
	static uint32_t gStart = 0;

	uint32_t now = millis();
	uint8_t center = fPixels.numPixels() / 2;

	if (!gInit) {
		for (uint8_t ii = 0 ; ii < kSpriteCount ; ii++) {
			gSprites[ii].fPos = random(fPixels.numPixels());
			gSprites[ii].fCount = 0;
			gSprites[ii].fStartTime = now + random(5000);
		}
		gStart = now;
		gInit = true;
	}

	SetStrip(0);
	for (uint8_t ii = 0 ; ii < kSpriteCount ; ii++) {
		if (gSprites[ii].fStartTime > now) continue;

		uint32_t color = fColors[ii % fColorCount];
		for (uint8_t jj = 1 ; jj <= kFireworkSize ; jj++) {
			int idx = jj * gSprites[ii].fCount;
			int idx1 = gSprites[ii].fPos + idx;
			if (idx1 < fPixels.numPixels()) fPixels.setPixelColor(idx1, color);

			int idx2 = gSprites[ii].fPos - idx;
			if (idx2 >= 0) fPixels.setPixelColor(idx2, color);
		}
		gSprites[ii].fCount++;
		if (gSprites[ii].fCount > fPixels.numPixels()) {
			gSprites[ii].fPos = random(fPixels.numPixels());
			gSprites[ii].fCount = 0;
			gSprites[ii].fStartTime = now + random(5000);
		}
	}
	fPixels.show();

	delay(delayMs);
}

void NeoPixo::SetStrip(uint32_t color) {
	for (uint8_t ii = 0 ; ii < fPixels.numPixels() ; ii++) {
		fPixels.setPixelColor(ii, color);
	}
}
