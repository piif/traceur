#ifndef HAVE_ARC_H
#define HAVE_ARC_H

#ifdef SIMULATE
#  include "simulate.h"
#else
#  include <Arduino.h>
#endif

// Initialize arc drawing :
// - from x0, y0
// - centered on xc,yc
// - until crossing segmant [(xc,yc) , (x1, y1)]
// - clockwise or counterclockwise
// incrX and incrY are filled with 1 or -1 to detail if x or y have to go forward or backward
void arcStart(int x0, int y0, int x1, int y1, int xc, int yc, boolean clockwise);

// After each call to arcStep, incrX and incrY are filled with :
//  0 : no move needed
//  1 : forward move
// -1 : backward move
// Furthermore, this function returns -1 if drawing is ended, 0 else
char arcStep(int *incrX, int *incrY);

#endif // HAVE_ARC_H
