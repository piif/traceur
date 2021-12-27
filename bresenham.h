#ifndef HAVE_BRESENHAM_H
#define HAVE_BRESENHAM_H

// Initialize segment drawing for relative coordinates dx,dy
// incrX and incrY are filled with 1 or -1 to detail if x or y have to go forward or backward
void bresenhamStart(int dx, int dy, char *incrX, char *incrY);

// After each call to bresenhamStep, incrX and incrY are filled with :
//  0 : no move needed
//  1 : forward move
// -1 : backward move
// Furthermore, this function returns -1 if drawing is ended, 0 else
char bresenhamStep(char *incrX, char *incrY);

#endif // HAVE_BRESENHAM_H
