#include "servo.h"
#include "pins.h"

// spec at https://www.rhydolabz.com/documents/22/SM_S2309S.pdf says PWN from 900 to 2100µs

boolean enabled = 0;
byte pin = 0;

void servoSetup(byte _pin) {
	pin = _pin;
	// disabled by default
	pinMode(pin, INPUT);

	// no OC1x output
	// WGM mode CTC with TOP=OCR1A = 0100 ?
	// prescale /8 = 010
	// interrupt on compare match A
	// OCR value

	// prepare timer
	TCCR1A = 0x00; // 0000 -- 00
	TCCR1B = 0x00; // 0x0A , init disabled // 00- 01 010
	TCCR1C = 0x00; // 00 ------
	TIMSK1 = 0x00; // 0x02 , init disabled
}

void servoStatus() {
	Serial.print("Pin   : "); Serial.println(pin);
	if (enabled) {
		Serial.print("Micros: "); Serial.println(OCR1A >> 1);
	} else {
		Serial.println("Disabled");
	}
}

ISR(TIMER1_vect) {
	static boolean output = 0;
	output = !output;
	digitalWrite(pin, output);
}

void servoSetMicros(unsigned int micros) {
	OCR1AH = micros >> 7;
	OCR1AL = micros << 1;
}

void servoEnable(boolean _enabled) {
	enabled = _enabled;
	// TODO : enable/disable interrupt
	if (enabled) {
		pinMode(pin, OUTPUT);
		TCCR1B = 0x0A;
		TIMSK1 = 0x02;
	} else {
		pinMode(pin, INPUT);
		TCCR1B = 0x00; // prescale 0 = disable timer
		TIMSK1 = 0x00;
	}
}
