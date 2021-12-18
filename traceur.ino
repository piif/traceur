#include <Arduino.h>
#include "serialInput.h"
#include "pins.h"
#include "move.h"

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

long speed = 100;

// in ms
long stepDelay = 1000L / speed;

void blink() {
	static byte debug_led=0;

	digitalWrite(DEBUG_PIN, debug_led);
	debug_led = !debug_led;
}

void high(byte v) {
	digitalWrite(v, HIGH);
}

void low(byte v) {
	digitalWrite(v, LOW);
}

void setSpeed(int v) {
	speed = v;
	stepDelay = 1000000L / speed;
}
void moveX(int v) {
	moveOf(v, 0);
//	stepperMoveOf(&X, v);
}
void moveY(int v) {
	moveOf(0, v);
//	stepperMoveOf(&Y, v);
}
//void moveZ(int v) {
//	stepperMoveOf(&Z, v);
//}
//void moveE(int v) {
//	stepperMoveOf(&E, v);
//}

void status() {
	Serial.print("Speed : "); Serial.println(speed);
	Serial.print("Delay : "); Serial.println(stepDelay);
	Serial.print("now   : "); Serial.println(micros());
	Serial.println();

	moveStatus();
}

void pen(byte v) {
	analogWrite(USER_3, v);
}

InputItem inputs[] = {
	{ '?', 'f', (void *)status },

	{ 's', 'I', (void *)setSpeed  },
	{ 'x', 'I', (void *)moveX },
	{ 'y', 'I', (void *)moveY },
//	{ 'z', 'I', (void *)moveZ },
//	{ 'e', 'I', (void *)moveE },

	{ 'p', 'B', (void *)pen },
	{ 'h', 'B', (void *)high },
	{ 'l', 'B', (void *)low },
//	{ 'r', 'f', (void *)setRemanent },
//	{ 'h', 'f', (void *)setHalf },
//	{ 'f', 'f', (void *)setFull }
};

void penUp() {

}
void penDown() {

}

void setup(void) {
	Serial.begin(DEFAULT_BAUDRATE);
	pinMode(USER_1, OUTPUT);
	pinMode(USER_2, OUTPUT);
	pinMode(USER_3, OUTPUT);
	pinMode(USER_4, OUTPUT);

	moveSetup();

	pinMode(DEBUG_PIN, OUTPUT);
	digitalWrite(DEBUG_PIN, HIGH);

	registerInput(sizeof(inputs), inputs);

	Serial.println("setup end");

	penUp();
	goOrigin();
}

void loop() {
//	static byte cpt = 0;
//	digitalWrite(USER_1, (cpt & 8) ? HIGH : LOW);
//	digitalWrite(USER_2, (cpt & 4) ? HIGH : LOW);
//	digitalWrite(USER_3, (cpt & 2) ? HIGH : LOW);
//	digitalWrite(USER_4, (cpt & 1) ? HIGH : LOW);
//	cpt++;

	if (minChanged) {
		status();
		Serial.println(minChanged);
		minChanged = 0;
	}
	handleInput();

//	byte moved = stepperDoStep(&X) || stepperDoStep(&Y) || stepperDoStep(&Z) || stepperDoStep(&E);
//	if (moved) {
//		blink();
//	}

	delay(stepDelay/1000);
//	delayMicroseconds(stepDelay); // !! delayMicroseconds doesn't work
}
