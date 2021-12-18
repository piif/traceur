#include <Arduino.h>

typedef struct _stepper {
	byte enable_pin, dir_pin, step_pin;
	long toMove;
} Stepper;

// initialize pins for stepper
void stepperInit(Stepper *s, byte enable_pin, byte dir_pin, byte step_pin);

// prepare for v steps , if v<0, direction is inverted
void stepperMoveOf(Stepper *s, int v);

// to one step , return 1 If a step was needed
byte stepperDoStep(Stepper *s);
