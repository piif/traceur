#include "move.h"
#include "stepper.h"

#define readMinX() (digitalRead(X_MIN_PIN) == 0)
#define readMinY() (digitalRead(Y_MIN_PIN) == 0)

Stepper X, Y /*, Z, E*/;

boolean minX = 0, minY = 0;
#define BOUNCE_DELAY 2000
long minXchangeTTL = 0, minYchangeTTL= 0;
int minChanged = 0;

int posX = MAX_X + 1, posY = MAX_Y + 1;

void moveOf(int dx, int dy) {
	if (posX + dx < 0) {
		dx = -posX;
	} else if (posX + dx > MAX_X) {
		dx = MAX_X - posX;
	}
	if (posY + dy < 0) {
		dy = -posY;
	} else if (posY + dy > MAX_Y) {
		dy = MAX_Y - posY;
	}
	posX += dx;
	posY += dy;
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
	cli();
	unsigned long now = micros();
	if (minXchangeTTL < now) {
		byte m = readMinX();
		if (m != minX) {
			minX = m;
			if (minX == 1) {
				X.toMove = 0;
				posX = 0;
			}
			minChanged++;
			minXchangeTTL = now+BOUNCE_DELAY;
		}
	}
	if (minYchangeTTL < now) {
		byte m = readMinY();
		if (m != minY) {
			minY = !minY;
			if (minY == 1) {
				Y.toMove = 0;
				posY = 0;
			}
			minChanged++;
			minYchangeTTL = now+BOUNCE_DELAY;
		}
	}
	sei();
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
	Serial.print("dx, X : "); Serial.print(X.toMove);Serial.print(", ");Serial.println(posX);
	Serial.print("dy, Y : "); Serial.print(Y.toMove);Serial.print(", ");Serial.println(posY);
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
	Y_MIN_PCMSK |= (1 << Y_MIN_INTR);
	PCICR |= (1 << X_MIN_PCICR) | (1 << Y_MIN_PCICR);

	Serial.print("PCICR  : "); Serial.println(PCICR, HEX);
	Serial.print("PCMSK0 : "); Serial.println(PCMSK0, HEX);
	Serial.print("PCMSK1 : "); Serial.println(PCMSK1, HEX);
	Serial.print("PCMSK2 : "); Serial.println(PCMSK2, HEX);
	Serial.print("PCMSK3 : "); Serial.println(PCMSK3, HEX);
}

