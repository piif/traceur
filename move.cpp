#include "move.h"
#include "stepper.h"
#include "bresenham.h"

#define readMinX() (digitalRead(X_MIN_PIN) == 0)
#define readMinY() (digitalRead(Y_MIN_PIN) == 0)

Stepper X, Y /*, Z, E*/;

boolean minX = 0, minY = 0;
#define BOUNCE_DELAY 2000
long minXchangeTTL = 0, minYchangeTTL= 0;
int minChanged = 0;

int posX = MAX_X + 1, posY = MAX_Y + 1;

void moveOf(int dx, int dy, boolean force) {
	if (force) {
		stepperMoveOf(&X, &Y, dx, dy);
		while(stepperDoStep(&X, &Y));
		return;
	}

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

	char incrX, incrY;
	bresenhamStart(dx, dy, &incrX, &incrY);
	stepperPrepare(&X, &Y, incrX < 0, incrY < 0);
	while(bresenhamStep(&incrX, &incrY) == 0) {
		stepperOneStep(&X, &Y, incrX != 0, incrY != 0);
	}
	stepperEnd(&X, &Y);
}

void moveToOrigin() {
	if (minX) {
		moveOf(50, 0, true);
	}
	if (minY) {
		moveOf(0, 50, true);
	}
	moveOf(-500, -500, true); // will stop on min detection
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

	moveStatus();
}

