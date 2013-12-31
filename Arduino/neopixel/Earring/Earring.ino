#include <Adafruit_NeoPixel.h>
#include <NeoPixo.h>

#define PIN 0

//#define __CHRISTMAS__
#define __NEWYEARS__

Adafruit_NeoPixel gStrip = Adafruit_NeoPixel(16, PIN);

uint8_t gMode = 4;
int gSpeed = 1;
uint32_t gLastModeChange;

#ifdef __CHRISTMAS__
#define kColorCount 3
uint32_t gColors[kColorCount] = {0xff0000, 0x00ff00, 0x0000ff};
#endif

#ifdef __NEWYEARS__
#define kColorCount 3
uint32_t gColors[kColorCount] = {0xffffff, 0x0000ff, 0xff30ff};
#endif

NeoPixo gPixo(gStrip, gColors, kColorCount);

void setup() {
	gStrip.begin();
	gStrip.setBrightness(60); // 1/3 brightness
	gLastModeChange = millis();
}

void loop() {
	switch (gMode) {
	case 0:
		gPixo.RandomSparks(1);
		break;
	case 1:
		gPixo.Spinny(2, 3, 50, gSpeed);
		break;
	case 2:
		gPixo.Race(3, gSpeed);
		delay(40);
		break;
	}

	uint32_t now = millis();
	if ((now - gLastModeChange) > 8000) {
		gMode++;
		if (gMode > 2) {
			gMode = 0;
			gSpeed = -gSpeed;
		}
		gLastModeChange = now;
	}
}
