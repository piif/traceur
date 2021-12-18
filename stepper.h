#ifndef HAVE_STEPPER_H
#define HAVE_STEPPER_H

#include <Arduino.h>

typedef struct _stepper {
	byte enable_pin, dir_pin, step_pin;
	long toMove;
} Stepper;

// initialize pins for stepper
void stepperInit(Stepper *s, byte enable_pin, byte dir_pin, byte step_pin);

// prepare for v steps , if v<0, direction is inverted
void stepperMoveOf(Stepper *s, int v);

// do one step , return 1 if a step was actually done
byte stepperDoStep(Stepper *s);

// initialize a stepper pair. Works also if enabled pin is shared
void stepperInit(Stepper *X, Stepper *Y,
		byte x_enable_pin, byte x_dir_pin, byte x_step_pin,
		byte y_enable_pin, byte y_dir_pin, byte y_step_pin);

void stepperMoveOf(Stepper *X, Stepper *Y,
		int dx, int dy);

byte stepperDoStep(Stepper *X, Stepper *Y);

#endif // HAVE_STEPPER_H
