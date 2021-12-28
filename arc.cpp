#include "arc.h"

byte quadrant(int x, int y) {
    if (x>=0) {
        if (y>=0) {
            return 0;
        } else {
            return 1;
        }
    } else {
        if (y<0) {
            return 2;
        } else {
            return 3;
        }
    }
}

struct _arcQ0 {
	int x, y, x1, y1, r, d;
	long E;
	boolean sign;
} arcQ0;

void arcStartQ0(int x0, int y0, int x1, int y1) {
	Serial.print("   Q0 ");Serial.print(x0);Serial.print(",");Serial.println(y0);
	Serial.print("   -> ");Serial.print(x1);Serial.print(",");Serial.println(y1);

	arcQ0.x = x0;
	arcQ0.y = y0;
	arcQ0.x1 = x1;
	arcQ0.y1 = y1;

    int r2 = x0*x0 + y0*y0;
    arcQ0.r = round( sqrt(r2) );

    arcQ0.E = (long)x1*y0 - (long)y1*x0;
    arcQ0.sign = (arcQ0.E > 0);

    arcQ0.d = arcQ0.r - 1;
}

char arcStepQ0(int *incrX, int *incrY) {
	if ( arcQ0.E == 0 || arcQ0.sign != (arcQ0.E > 0) ) {
		*incrX = *incrY = 0;
		return -1;
	} else if ( arcQ0.d >= 2 * arcQ0.x ) {
		arcQ0.d = arcQ0.d - 2 * arcQ0.x - 1;
		arcQ0.x = arcQ0.x + 1;
		arcQ0.E -= arcQ0.y1;
		*incrX = 1;
		*incrY = 0;
		return 0;
	} else if ( arcQ0.d < 2 * (arcQ0.r - arcQ0.y) ) {
		arcQ0.d = arcQ0.d + 2 * arcQ0.y - 1;
		arcQ0.y = arcQ0.y - 1;
		arcQ0.E -= arcQ0.x1;
		*incrX = 0;
		*incrY = -1;
		return 0;
	} else {
		arcQ0.d = arcQ0.d + 2 * (arcQ0.y - arcQ0.x - 1);
		arcQ0.y = arcQ0.y - 1;
		arcQ0.x = arcQ0.x + 1;
		arcQ0.E -= arcQ0.x1 + arcQ0.y1;
		*incrX = 1;
		*incrY = -1;
		return 0;
	}
}

typedef struct _arcPart {
	int x0, y0, x1, y1;
	char dx, dy;
	boolean invert;
} ArcPart;

ArcPart arcPart[5];
char currentQ = 0;
byte lastQ;

void initArcPart(ArcPart *arcPart, byte qd, boolean clockwise) {
	arcPart->x0 = 0;
	arcPart->y0 = 0;
	arcPart->x1 = 1;
	arcPart->y1 = 0;
	switch (qd) {
	case 0:
		arcPart->dx = clockwise ? 1 : -1;
		arcPart->dy = 1;
		arcPart->invert = 0;
	break;
	case 1:
		arcPart->dx = -1;
		arcPart->dy = clockwise ? 1 : -1;
		arcPart->invert = 1;
	break;
	case 2:
		arcPart->dx = clockwise ? -1 : 1;
		arcPart->dy = -1;
		arcPart->invert = 0;
	break;
	case 3:
		arcPart->dx = 1;
		arcPart->dy = clockwise ? -1 : 1;
		arcPart->invert = 1;
	break;
	}
}

void computeArcCoord(ArcPart *arcPart, int xSrc, int ySrc, int *xDst, int *yDst, boolean init = 0) {
	if (arcPart->invert) {
		if (init) {
			*xDst = ySrc * arcPart->dx;
			*yDst = xSrc * arcPart->dy;
		} else {
			*xDst = ySrc * arcPart->dy;
			*yDst = xSrc * arcPart->dx;
		}
	} else {
		*xDst = xSrc * arcPart->dx;
		*yDst = ySrc * arcPart->dy;
	}
}

// Initialize arc drawing :
// - from x0, y0
// - centered on xc,yc
// - until crossing segmant [(xc,yc) , (x1, y1)]
// - clockwise or counterclockwise
// incrX and incrY are filled with 1 or -1 to detail if x or y have to go forward or backward
void arcStart(int x0, int y0, int x1, int y1, int xc, int yc, boolean clockwise) {
	int xx0 = clockwise ? x0 - xc : xc - x0;
	int yy0 = y0 - yc;
	int xx1 = clockwise ? x1 - xc : xc - x1;
	int yy1 = y1 - yc;

	byte qi = quadrant(xx0, yy0);
	byte qf = quadrant(xx1, yy1);
	Serial.print(" 0: ");Serial.print(xx0);Serial.print(",");Serial.println(yy0);
	Serial.print(" 1: ");Serial.print(xx1);Serial.print(",");Serial.println(yy1);
	Serial.print(" q: ");Serial.print(qi);Serial.print(" -> ");Serial.println(qf);

	initArcPart(&arcPart[0], qi, clockwise);
	computeArcCoord(&arcPart[0], xx0, yy0, &(arcPart[0].x0), &(arcPart[0].y0), 1);
	if (qi == qf) {
		Serial.print(" vect ");Serial.print((long)xx0*yy1);Serial.print(",");Serial.println((long)yy0*xx1);
		if ((long)xx0*yy1 > (long)yy0*xx1) {
			for (byte i = 1; i <= 4; i++) {
				initArcPart(&arcPart[i], (qi+i) % 4, clockwise);
			}
			lastQ = 4;
		} else {
			lastQ = 0;
		}
	} else {
		lastQ = 0;
		do {
			lastQ++;
			qi = (qi + 1) % 4;
			initArcPart(&arcPart[lastQ], qi, clockwise);
		} while (qi != qf);
	}
	computeArcCoord(&arcPart[lastQ], xx1, yy1, &(arcPart[lastQ].x1), &(arcPart[lastQ].y1), 1);

	for(byte i = 0; i <= lastQ; i++) {
		Serial.print("  arc ");Serial.print(arcPart[i].x0);Serial.print(",");Serial.println(arcPart[i].y0);
		Serial.print("   -> ");Serial.print(arcPart[i].x1);Serial.print(",");Serial.println(arcPart[i].y1);
		Serial.print("    d ");Serial.print((int)(arcPart[i].dx));Serial.print(",");Serial.println((int)(arcPart[i].dy));
		Serial.print("  inv ");Serial.println(arcPart[i].invert);
	}
	currentQ = -1;
}

// After each call to arcStep, incrX and incrY are filled with :
//  0 : no move needed
//  1 : forward move
// -1 : backward move
// Furthermore, this function returns -1 if drawing is ended, 0 else
char arcStep(int *incrX, int *incrY) {
	if (currentQ == -1) {
		currentQ = 0;
		ArcPart *a = &(arcPart[currentQ]);
		arcStartQ0(a->x0, a->y0, a->x1, a->y1);
	} else if (currentQ > lastQ) {
		*incrX = *incrY = 0;
		return -1;
	}
	char r = arcStepQ0(incrX, incrY);
	if (r == -1) {
		currentQ++;
		if (currentQ > lastQ) {
			*incrX = *incrY = 0;
			return -1;
		}
		ArcPart *a = &(arcPart[currentQ]);
		arcStartQ0(arcQ0.y, arcQ0.x, a->x1, a->y1);
		r = arcStepQ0(incrX, incrY);
	}
	computeArcCoord(&arcPart[currentQ], *incrX, *incrY, incrX, incrY);
	return r;
}

#ifdef SIMULATE
int main(int argc, char **argv) {
	int x0, y0, x1, y1, xc = 0, yc = 0, clockwise = 1;

	switch(argc) {
	case 5:
		break;
	case 6:
		clockwise = atoi(argv[5]);
		break;
	case 7:
		xc = atoi(argv[5]);
		yc = atoi(argv[6]);
		break;
	case 8:
		clockwise = atoi(argv[5]);
		xc = atoi(argv[6]);
		yc = atoi(argv[7]);
		break;
	default:
		cerr << argv[0] << " x0 y0 x1 y1 [ clockwise ] [ xc yc ]" << endl;
		exit(1);
	}
	x0 = atoi(argv[1]);
	y0 = atoi(argv[2]);
	x1 = atoi(argv[3]);
	y1 = atoi(argv[4]);

	cout << x0 << "," << y0 << " " << x1 << ","  << y1 << " "  << xc << ","  << yc << " / "  << clockwise << endl;
	arcStart(x0, y0, x1, y1, xc, yc, clockwise);

	int dx, dy;
	while(arcStep(&dx, &dy) != -1) {
		Serial.print("move ");Serial.print(dx);Serial.print(" , ");Serial.println(dy);
	}
	return 0;
}
#endif
