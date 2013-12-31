#include <Adafruit_NeoPixel.h>
#include <NeoPixo.h>
#include <CommandStream.h>

#define PIN 6
#define kModeCount 6

//#define __CHRISTMAS__
#define __NEWYEARS__

Adafruit_NeoPixel gStrip = Adafruit_NeoPixel(238, PIN);
CommandStream gCommand;

#ifdef __CHRISTMAS__
#define kColorCount 3
uint32_t gColors[kColorCount] = {0xff0000, 0x00ff00, 0x0000ff};
#endif

#ifdef __NEWYEARS__
#define kColorCount 3
uint32_t gColors[kColorCount] = {0xffffff, 0x0000ff, 0xff30ff};
#endif

#define kFireworksColorCount 7
uint32_t gFireworksColors[kFireworksColorCount] = {0xffffff, 0x0000ff, 0x00ff00, 0xff0000, 0xffff00, 0xff00ff, 0x00ffff};

NeoPixo gPixo(gStrip, gColors, kColorCount);
NeoPixo gFireworks(gStrip, gFireworksColors, kFireworksColorCount);

uint8_t gMode = 0;
bool gSequence = true;
int gSpeed = 1;
uint32_t gChangeEvery = 0;
uint32_t gLastModeChange = 0;
uint8_t gCount = 0;

void setup() {
	Serial.begin(115200);
	gStrip.begin();
	gStrip.setBrightness(60); // 1/3 brightness
	gLastModeChange = millis();
}

char gBuf[10];
int gBufPos = 0;
int gBufSize = 0;

void CheckSerial() {
	if (Serial.available() == 0) {
		return;
	}

	char cc = Serial.read();
	gBuf[gBufPos++] = cc;
	if (gBufPos >= 10) gBufPos = 0;
	if (gBufSize < 10) gBufSize++;

	gCommand.HandleChar(cc);
	if (!gCommand.HasCommand()) {
		return;
	}

	int value = gCommand.GetValue();
	switch (gCommand.GetCommand()) {
	case 'm':
		gMode = gCommand.GetValue();
		break;
	case 'r':
		gChangeEvery = gCommand.GetValue();
		gSequence = false;
		gLastModeChange = millis() - gChangeEvery;
		break;
	case 's':
		gChangeEvery = gCommand.GetValue();
		gSequence = true;
		gLastModeChange = millis() - gChangeEvery;
		break;
	case 'd':
		gSpeed = (gCommand.GetValue() == 0) ? 1 : -1;
		break;
	case 'c':
		gCount = gCommand.GetValue();
		break;
	case 'i': {
		Serial.println(gChangeEvery);
		char buf[11];
		int ii;
		for (ii = 0 ; ii < gBufSize ; ii++) {
			buf[ii] = gBuf[(ii+gBufPos)%10];
		}
		buf[ii] = 0;
		Serial.println(buf);
	} break;
	}
}

void loop() {
	switch (gMode) {
	case 0:
		gPixo.Off();
		break;
	case 1:
		gPixo.RandomSparks(3);
		break;
	case 2:
		gPixo.Spinny(2, 3, 50, gSpeed);
		break;
	case 3:
		gPixo.Spinny(8, 3, 50, gSpeed);
		break;
	case 4:
		gPixo.Spinny(1, 2, 20, gSpeed);
		break;
	case 5:
		gPixo.Rainbow(2, gSpeed);
		break;
	case 6:
		gPixo.Race(10, gSpeed);
		break;
	case 97:
		gFireworks.Fireworks(10);
		break;
	case 98:
		gFireworks.Countdown(gCount, 2, 0);
		break;
	case 99:
		gPixo.Lightning();
		break;
	}

	if (gChangeEvery > 0) {
		uint32_t now = millis();
		if ((now - gLastModeChange) > gChangeEvery) {
			if (gSequence) {
				gMode++;
				if (gMode > kModeCount) {
					gMode = 1;
					gSpeed = -gSpeed;
				}
			}
			else {
				gMode = random(kModeCount - 1) + 1;
				gSpeed = random(2) == 0 ? 1 : -1;
			}
			gLastModeChange = now;
		}
	}
	CheckSerial();
}

