#include <Servo.h>

Servo gServo;

#define kServoPin 9
#define kServoMin 0
#define kServoMax 130

#define kSwitchPin 10

uint32_t gLastTrigger = 0;
int gCount = 0;

#define kFactorMin 1
#define kFactorMax 5

#define kStage2 2

void Attach() {
	if (gServo.attached()) return;
	gServo.attach(kServoPin);
	gServo.write(kServoMax);
}

void Detach() {
	if (!gServo.attached()) return;
	gServo.detach();
}

void setup() { 
	pinMode(kSwitchPin, INPUT);
	digitalWrite(kSwitchPin, HIGH);
	Attach();
}

void loop() {
	uint32_t now = millis();
	if (digitalRead(kSwitchPin) == LOW) {
		Attach();
		gCount++;
		gLastTrigger = now;
		if (gCount <= kStage2) {
			delay(50);
		} else {
			while (random(gCount - kStage2 + 2) != 0) {
				uint32_t factor = random(kFactorMin, kFactorMax + 1);
				gServo.write(((kFactorMax - factor) * kServoMin + factor * kServoMax) / kFactorMax);
				delay(500 * random(1, 5));
			}
		}
		gServo.write(kServoMin);
		delay(400);
		gServo.write(kServoMax);
		delay(300);
		return;
	}
	if (now - gLastTrigger > 20 * 1000L) {
		Detach();
		gCount = 0;
	}
	randomSeed(random(__LONG_MAX__) ^ micros());
}
