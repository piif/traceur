#ifndef HAVE_MOVE_H
#define HAVE_MOVE_H

#include <Arduino.h>
#include "pins.h"

#define MAX_X 500
#define MAX_Y 500

extern int posX, posY;
extern int minChanged;

void moveSetup();
void moveStatus();

void moveOf(int dx, int dy, boolean force = false) ;

void moveToOrigin();

void moveArc(int x0, int y0, int x1, int y1, int xc, int yc, boolean clockwise);

#endif // HAVE_MOVE_H
