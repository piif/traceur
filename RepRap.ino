#include <Arduino.h>
#include "serialInput.h"
#include "stepper.h"

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

// FROM arduino pins.h
// D0  = PB0
// ..
// D7  = PB7
// D8  = PD0
// ..
// D15 = PD7
// D16 = PC0
// ..
// D23 = PC7
// D24 = PA0
// ..
// D31 = PA7

// FROM https://github.com/kliment/Sprinter/blob/master/Sprinter/pins.h#L301
// x axis
#define X_STEP_PIN 15
#define X_DIR_PIN 18
#define X_ENABLE_PIN 31 // doc says 24 !
#define X_MIN_PIN 20

// y axes
#define Y_STEP_PIN 23
#define Y_DIR_PIN 22
#define Y_ENABLE_PIN 31 // doc says 24 !
#define Y_MIN_PIN 30

// z axes
#define Z_STEP_PIN 28
#define Z_DIR_PIN 27
#define Z_ENABLE_PIN 29
#define Z_MIN_PIN 25

//extruder pins
#define E_STEP_PIN 12
#define E_DIR_PIN 17
#define E_ENABLE_PIN 3

#define DEBUG_PIN 0

// LOW should open mofset linked to FAN connector
#define FAN_PIN 2

// 5 lines of 3 pins (2 for bottom one) at bottom left corner
// see http://reprap.org/wiki/Monotronics#Pinout_Maps
// pinout :
// GND +5V D26/A5
// GND +5V D1
// GND +5V ARef
// GND +5V D4
// GND D31/A0
#define USER_1 26
#define USER_2 1
#define USER_3 4
#define USER_4 31

Stepper X, Y, Z, E;

long speed = 100;
long stepDelay = 1000000L / speed;

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
	stepperMoveOf(&X, v);
}
void moveY(int v) {
	stepperMoveOf(&Y, v);
}
void moveZ(int v) {
	stepperMoveOf(&Z, v);
}
void moveE(int v) {
	stepperMoveOf(&E, v);
}

void status() {
	Serial.print("Speed : "); Serial.println(speed);
	Serial.print("Delay : "); Serial.println(stepDelay);
	Serial.print("milli : "); Serial.println(millis());
	Serial.println();

	Serial.print("    X : "); Serial.println(X.toMove);
	Serial.print("    Y : "); Serial.println(Y.toMove);
	Serial.print("    Z : "); Serial.println(Z.toMove);
	Serial.print("    E : "); Serial.println(E.toMove);
	Serial.println();

	Serial.print("min X : "); Serial.println(digitalRead(X_MIN_PIN));
	Serial.print("min Y : "); Serial.println(digitalRead(Y_MIN_PIN));
	Serial.print("min Z : "); Serial.println(digitalRead(Z_MIN_PIN));
}

InputItem inputs[] = {
	{ '?', 'f', (void *)status },

	{ 's', 'I', (void *)setSpeed  },
	{ 'x', 'I', (void *)moveX },
	{ 'y', 'I', (void *)moveY },
	{ 'z', 'I', (void *)moveZ },
	{ 'e', 'I', (void *)moveE },

	{ 'h', 'B', (void *)high },
	{ 'l', 'B', (void *)low },
//	{ 'r', 'f', (void *)setRemanent },
//	{ 'h', 'f', (void *)setHalf },
//	{ 'f', 'f', (void *)setFull }
};

void setup(void) {
	Serial.begin(DEFAULT_BAUDRATE);
	pinMode(USER_1, OUTPUT);
	pinMode(USER_2, OUTPUT);
	pinMode(USER_3, OUTPUT);
	pinMode(USER_4, OUTPUT);

	stepperInit(&X, X_ENABLE_PIN, X_DIR_PIN, X_STEP_PIN);
	pinMode(X_MIN_PIN, INPUT_PULLUP);

	stepperInit(&Y, Y_ENABLE_PIN, Y_DIR_PIN, Y_STEP_PIN);
	pinMode(Y_MIN_PIN, INPUT_PULLUP);

	stepperInit(&Z, Z_ENABLE_PIN, Z_DIR_PIN, Z_STEP_PIN);
	pinMode(Z_MIN_PIN, INPUT_PULLUP);

	stepperInit(&E, E_ENABLE_PIN, E_DIR_PIN, E_STEP_PIN);

	pinMode(DEBUG_PIN, OUTPUT);
	digitalWrite(DEBUG_PIN, HIGH);

	registerInput(sizeof(inputs), inputs);
	Serial.print("F_CPU = "); Serial.println(F_CPU);
	Serial.println("setup end");
}

void loop() {
//	static byte cpt = 0;
//	digitalWrite(USER_1, (cpt & 8) ? HIGH : LOW);
//	digitalWrite(USER_2, (cpt & 4) ? HIGH : LOW);
//	digitalWrite(USER_3, (cpt & 2) ? HIGH : LOW);
//	digitalWrite(USER_4, (cpt & 1) ? HIGH : LOW);
//	cpt++;

	handleInput();

	byte moved = stepperDoStep(&X) || stepperDoStep(&Y) || stepperDoStep(&Z) || stepperDoStep(&E);
	if (moved) {
		blink();
	}

	delay(stepDelay/1000);
//	delayMicroseconds(stepDelay); // !! delayMicroseconds doesn't work
}
