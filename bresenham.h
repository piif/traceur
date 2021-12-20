
// initialize segment drawing
bresenhamStart(int dx, int dy);

// for each call to bresenhamStep, X must be incremented
// this function returns :
// -1 if drawing is ended
// +1 if Y must be incremented
//  0 if Y doesn't change
char bresenhamStep();
