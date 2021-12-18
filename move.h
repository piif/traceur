#ifndef HAVE_MOVE_H
#define HAVE_MOVE_H

#include <Arduino.h>
#include "pins.h"

#define MAX_X 500
#define MAX_Y 500

extern boolean minChanged;

void moveSetup();
void moveStatus();

void moveOf(int dx, int dy) ;

void goOrigin();

#endif // HAVE_MOVE_H
