#include "servo.h"
#include "pins.h"

// spec at https://www.rhydolabz.com/documents/22/SM_S2309S.pdf says PWN from 900 to 2100µs

// seems we have 12,5MHz Frequency ?
// clock/32 and OCR2A = 250 => 640µs

boolean enabled = 0;
byte pin = 0;

void servoSetup(byte _pin) {
	pin = _pin;
	// disabled by default
	pinMode(pin, OUTPUT);

	// no OC1x output
	// WGM mode CTC with TOP=OCR2A = 010 ?
	// prescale /128 = 101
	// interrupt on compare match A
	// OCR value

	// prepare timer
	TCCR1A = 0x10; // 00 01 -- 00
	TCCR1B = 0x08; // init disabled, else 0x09 : 00 - 01 001
	//TIMSK1 = 0x00; // init disabled, else 0x04 : ----- 100
}

void servoStatus() {
	Serial.print("Pin   : "); Serial.println(pin);
	if (enabled) {
		Serial.print("Micros: "); Serial.println(OCR1B);
	} else {
		Serial.println("Disabled");
	}
}

ISR(TIMER2_vect) {
	static boolean output = 0;
	output = !output;
	digitalWrite(pin, output);
}

void servoSetMicros(unsigned int micros) {
	OCR1B = micros;
//	OCR1BH = (byte)(micros >> 8);
//	OCR1BL = (byte)(micros);
}

void servoEnable(boolean _enabled) {
	enabled = _enabled;
	// TODO : enable/disable interrupt
	if (enabled) {
//		pinMode(pin, OUTPUT);
		TCCR1B = 0x09; // Clk/1
		//TIMSK1 = 0x04;
	} else {
//		pinMode(pin, INPUT);
		TCCR1B = 0x08; // prescale 0 = disable timer
		//TIMSK1 = 0x00;
	}
}
