#include <Arduino.h>
#include "pins.h"
#include "move.h"
#include "servo.h"

#define GCODE_MODE


#ifdef GCODE_MODE
#  include "gcode.h"
extern gcode GCode;
#else
#  include "serialInput.h"
#endif

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

long speed = 100;

// in ms
long stepDelay = 1000L / speed;

void blink() {
	static byte debug_led=0;

	digitalWrite(DEBUG_PIN, debug_led);
	debug_led = !debug_led;
}

void high(byte v) {
	pinMode(v, OUTPUT);
	digitalWrite(v, HIGH);
}

void low(byte v) {
	digitalWrite(v, LOW);
}

void setSpeed(int v) {
	speed = v;
	stepDelay = 1000000L / speed;
}
void moveX(int v) {
	moveOf(v, 0);
}
void moveY(int v) {
	moveOf(0, v);
}

boolean penIsUp = 0;

void penUp(boolean force = false) {
	Serial.println("penUp");
	if (!force && penIsUp) {
		return;
	}
	servoSetMicros(2700);
	delay(500);
	servoSetMicros(2600);
	penIsUp = 1;
}

void penDown(boolean force = false) {
	Serial.println("penDown");
	if (!force && !penIsUp) {
		return;
	}
	servoSetMicros(1800);
	delay(500);
	penIsUp = 0;
}

void drawTest() {
	penDown();

	moveOf(300, 0);
	moveOf(0, 300);
	moveOf(-300, 0);
	moveOf(0, -300);

	moveOf(300, 300);
	moveOf(150, -150);
	moveOf(-150, -150);
	moveOf(-300, 300);

	penUp();
}

void status() {
	Serial.print("Speed : "); Serial.println(speed);
	Serial.print("Delay : "); Serial.println(stepDelay);
	Serial.print("now   : "); Serial.println(micros());

	Serial.println();
	servoStatus();

	Serial.println();
	moveStatus();
}

void home() {
	Serial.println("home");
	penUp(true);
	moveToOrigin();
}

void moveTo() {
	Serial.println("moveTo");
    if(GCode.availableValue('Z')) {
    	double z = GCode.GetValue('Z');
    	if (z > 0 && !penIsUp) {
    		penUp();
    	}
    	if (z < 0 && penIsUp) {
    		penDown();
    	}
    }

    double x, y;
    if(GCode.availableValue('X')) {
    	x = GCode.GetValue('X');
    } else {
    	x = posX;
    }
    if(GCode.availableValue('Y')) {
    	y = GCode.GetValue('Y');
    } else {
    	y = posY;
    }
	Serial.print(" -> moveOf "); Serial.print(x - posX); Serial.print(" , "); Serial.print(y - posY);
    moveOf(x - posX , y - posY);
}

void defaultCallback() {
	Serial.println("defaultCallback");
	for(byte i = 0; i < gcode_Buffer_size; i++) {
		if (GCode.BufferList[i].command != '\0') {
			Serial.print(GCode.BufferList[i].command); Serial.print(' '); Serial.println(GCode.BufferList[i].Value);
		}
	}
}

#ifdef GCODE_MODE

commandscallback commands[] = {
		{ "G28", home },
		{ "M114", status },
		{ "G0" , moveTo },
		{ "G1" , moveTo }
};
gcode GCode(sizeof(commands)/sizeof(commands[0]), commands, defaultCallback);

#else

InputItem inputs[] = {
	{ '?', 'f', (void *)status },
	{ 't', 'f', (void *)drawTest },

	{ '0', 'f', (void *)home },
	{ 's', 'I', (void *)setSpeed  },
	{ 'x', 'I', (void *)moveX },
	{ 'y', 'I', (void *)moveY },

	{ 'p', 'I', (void *)servoSetMicros },
	{ 'h', 'B', (void *)high },
	{ 'l', 'B', (void *)low },
};

#endif

void setup(void) {
	Serial.begin(DEFAULT_BAUDRATE);

	moveSetup();

	servoSetup(SERVO);

	pinMode(DEBUG_PIN, OUTPUT);
	digitalWrite(DEBUG_PIN, HIGH);

#ifdef GCODE_MODE
	GCode.begin(DEFAULT_BAUDRATE);
#else
	registerInput(sizeof(inputs), inputs);
#endif

	Serial.println("setup end");

	home();

	Serial.println("init end");
}

void loop() {
#ifdef GCODE_MODE
	GCode.available();
#else
	if (minChanged) {
		status();
		Serial.println(minChanged);
		minChanged = 0;
	}
	handleInput();

	delay(stepDelay/1000);
//	delayMicroseconds(stepDelay); // !! delayMicroseconds doesn't work
#endif
}
