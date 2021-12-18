#include "stepper.h"

void stepperInit(Stepper *s, byte enable_pin, byte dir_pin, byte step_pin) {
	s->toMove = 0;
	s->step_pin = step_pin;
	s->dir_pin = dir_pin;
	s->enable_pin = enable_pin;

	pinMode(s->step_pin, OUTPUT);
	pinMode(s->dir_pin, OUTPUT);
	pinMode(s->enable_pin, OUTPUT);
	digitalWrite(s->enable_pin, 1);
}

void stepperMoveOf(Stepper *s, int v) {
	digitalWrite(s->dir_pin, (v > 0));
	digitalWrite(s->step_pin, 0);
	digitalWrite(s->enable_pin, 0);

	s->toMove = v;
}

// to one step , return 1 If a step was needed
byte stepperDoStep(Stepper *s) {
	if (s->toMove != 0) {
		digitalWrite(s->step_pin, 1);
		delay(1);
//		delayMicroseconds(250);
		digitalWrite(s->step_pin, 0);
		delay(1);
//		delayMicroseconds(250);
		if (s->toMove> 0) {
			s->toMove--;
		} else {
			s->toMove++;
		}
		if (s->toMove == 0) {
			digitalWrite(s->enable_pin, 1);
		}
		return 1;
	}
	return 0;
}
