#include <Arduino.h>
#include <avr/boot.h>
#include "serialInput.h"
#include "pins.h"
#include "move.h"
#include "servo.h"

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
	pinMode(v, OUTPUT);
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
	servoStatus();

	Serial.println();
	moveStatus();
}

void pen(int v) {
	if (v == 0) {
		servoEnable(0);
	} else {
		servoEnable(1);
	}
	servoSetMicros(v);
}

InputItem inputs[] = {
	{ '?', 'f', (void *)status },

	{ 's', 'I', (void *)setSpeed  },
	{ 'x', 'I', (void *)moveX },
	{ 'y', 'I', (void *)moveY },
//	{ 'z', 'I', (void *)moveZ },
//	{ 'e', 'I', (void *)moveE },

	{ 'p', 'I', (void *)pen },
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

	Serial.print("TCCR0A = "); Serial.println(TCCR0A, HEX); // 3 = WGM 3 = fast PWM TOP
	Serial.print("TCCR0B = "); Serial.println(TCCR0B, HEX); // 3 = prescale 3 = /64
	Serial.print("TIMSK0 = "); Serial.println(TIMSK0, HEX); // 1 = OCIE0A
	Serial.print("OCR0A  = "); Serial.println(OCR0A); //
	Serial.println();
	Serial.print("TCCR1A = "); Serial.println(TCCR1A, HEX); // 1
	Serial.print("TCCR1B = "); Serial.println(TCCR1B, HEX); // 3
	Serial.print("TCCR1C = "); Serial.println(TCCR1C, HEX); // 0
	Serial.print("TIMSK1 = "); Serial.println(TIMSK1, HEX); // 0
	Serial.println();
	Serial.print("TCCR2A = "); Serial.println(TCCR2A, HEX); // 1 = WGM Phase correct
	Serial.print("TCCR2B = "); Serial.println(TCCR2B, HEX); // 4 = /64
	Serial.print("TIMSK2 = "); Serial.println(TIMSK2, HEX); // 0
	Serial.println();

	moveSetup();
	servoSetup(12 /*SERVO*/);

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
