// from https://fr.wikipedia.org/wiki/Algorithme_de_trac%C3%A9_de_segment_de_Bresenham#:~:text=L'algorithme%20de%20trac%C3%A9%20de,attach%C3%A9%20%C3%A0%20une%20console%20texte.

int DX, DY, E, Count;
char inverted, incrX, incrY;

void simpleBresenhamStart(int dx, int dy) {
	Count = dx;
	E = dx;
	DX = dx << 1;
	DY = dy << 1;
}

char simpleBresenhamStep() {
	if (Count == 0) {
		return -1;
	}
	Count--;
	E -= DY;
	if (E < 0) {
		E += DX;
		return 1;
	} else {
		return 0;
	}
}

void bresenhamStart(int dx, int dy, char *_incrX, char *_incrY) {
	if (dx == 0 && dy == 0) {
		Count = 0;
		return;
	}
	if (dx > 0) {
		*_incrX = incrX = 1;
	} else {
		*_incrX = incrX = -1;
		dx = -dx;
	}
	if (dy > 0) {
		*_incrY = incrY = 1;
	} else {
		*_incrY = incrY = -1;
		dy = -dy;
	}
	if (dy > dx) {
		inverted = 1;
		simpleBresenhamStart(dy, dx);
	} else {
		inverted = 0;
		simpleBresenhamStart(dx, dy);
	}
}

char bresenhamStep(char *_incrX, char *_incrY) {
	char s=simpleBresenhamStep();
	if (s == -1) {
		*_incrX = *_incrY = 0;
		return -1;
	}
	if (inverted) {
		*_incrX = s ? incrX : 0;
		*_incrY = incrY;
	} else {
		*_incrX = incrX;
		*_incrY = s ? incrY : 0;
	}
	return 0;
}

#ifdef TEST
#include <stdio.h>

void testBresenham(int dx, int dy) {
	printf("draw % 2d / % 2d\n", dx, dy);
	char ix, iy;
	bresenhamStart(dx, dy, &ix, &iy);
	printf("  => % 2d / % 2d\n", ix, iy);
	int X=0, Y=0;
	printf("% 2d / % 2d\n", X, Y);
	for(;;) {
		bresenhamStep(&ix, &iy);
		if (ix == 0 && iy == 0) {
			puts("end.");
			return;
		}
		X += ix;
		Y += iy;
		printf("% 2d / % 2d\n", X, Y);
	}
}

int main() {
	testBresenham( 7, 3);
	testBresenham( 3, 7);
	testBresenham(-3, 7);
	testBresenham(-7, 3);
	testBresenham(-7,-3);
	testBresenham(-3,-7);
	testBresenham( 3,-7);
	testBresenham( 7,-3);
	testBresenham( 0, 3);
	testBresenham( 3, 0);
	testBresenham(-3, 0);
	testBresenham( 0, 0);

	return 0;
}
#endif
