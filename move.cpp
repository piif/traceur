#include "move.h"
#include "stepper.h"

#define readMinX() (digitalRead(X_MIN_PIN) == 0)
#define readMinY() (digitalRead(Y_MIN_PIN) == 0)

Stepper X, Y /*, Z, E*/;
boolean minX = 0, minY = 0;
boolean minChanged = 0;
int posX, posY;

void moveOf(int dx, int dy) {
	stepperMoveOf(&X, &Y, dx, dy);
	// TODO : add speed + Bresenham
	while(stepperDoStep(&X, &Y));
}

void goOrigin() {
	if (minX) {
		moveOf(50, 0);
	}
	if (minY) {
		moveOf(0, 50);
	}
	moveOf(-500, -500);
	posX = 0;
	posY = 0;
}


void checkMin() {
	if (readMinX() != minX) {
		minX = !minX;
		if (minX == 0) {
			X.toMove = 0;
		}
		minChanged = 1;
	}
	if (readMinY() != minY) {
		minY = !minY;
		if (minY == 0) {
			Y.toMove = 0;
		}
		minChanged = 1;
	}
}

ISR(X_MIN_PCINT_vect) {
	checkMin();
}
//#if Y_MIN_PCICR != X_MIN_PCICR
ISR(Y_MIN_PCINT_vect) {
	checkMin();
}
//#endif

void moveStatus() {
	Serial.print("    X : "); Serial.println(X.toMove);
	Serial.print("    Y : "); Serial.println(Y.toMove);
//	Serial.print("    Z : "); Serial.println(Z.toMove);
//	Serial.print("    E : "); Serial.println(E.toMove);
	Serial.println();

	Serial.print("min X : "); Serial.println(minX);
	Serial.print("min Y : "); Serial.println(minY);
//	Serial.print("min Z : "); Serial.println(digitalRead(Z_MIN_PIN));
}

void moveSetup() {
	stepperInit(&X, X_ENABLE_PIN, X_DIR_PIN, X_STEP_PIN);
	pinMode(X_MIN_PIN, INPUT_PULLUP);

	stepperInit(&Y, Y_ENABLE_PIN, Y_DIR_PIN, Y_STEP_PIN);
	pinMode(Y_MIN_PIN, INPUT_PULLUP);

//	stepperInit(&Z, Z_ENABLE_PIN, Z_DIR_PIN, Z_STEP_PIN);
//	pinMode(Z_MIN_PIN, INPUT_PULLUP);
//
//	stepperInit(&E, E_ENABLE_PIN, E_DIR_PIN, E_STEP_PIN);

	minX = readMinX();
	minY = readMinY();

	X_MIN_PCMSK |= (1 << X_MIN_INTR);
	Y_MIN_PCMSK |= 0x40; // (1 << Y_MIN_INTR);
	PCICR |= (1 << X_MIN_PCICR) | (1 << Y_MIN_PCICR);

	Serial.print("PCICR  : "); Serial.println(PCICR);
	Serial.print("PCMSK0 : "); Serial.println(PCMSK0);
	Serial.print("PCMSK1 : "); Serial.println(PCMSK1);
	Serial.print("PCMSK2 : "); Serial.println(PCMSK2);
	Serial.print("PCMSK3 : "); Serial.println(PCMSK3);
}

