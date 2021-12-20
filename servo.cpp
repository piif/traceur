#include "servo.h"
#include "pins.h"

// spec at https://www.rhydolabz.com/documents/22/SM_S2309S.pdf says PWN from 900 to 2100µs

// seems we have 12,5MHz Frequency ?
// clock/32 and OCR2A = 250 => 640µs

boolean enabled = 0;
volatile byte pin = 0;
volatile boolean output = 0;

void servoSetup(byte _pin) {
	pin = _pin;
	// disabled by default
	pinMode(pin, OUTPUT);

	// no OC1x output
	// WGM mode CTC with TOP=OCR1A = 010 ?
	// prescale /128 = 101
	// interrupt on compare match A
	// OCR value

	cli();

	// prepare timer
	TCCR1A = 0x00; // 00 01 -- 00
	TCCR1B = 0x00; // init disabled, else 0x0A : 00 - 01 001
	TIMSK1 = 0x00; // init disabled, else 0x02 : ----- 100

	sei();
}

void servoStatus() {
	Serial.print("Pin   : "); Serial.println(pin);
	if (enabled) {
		Serial.print("Micros: "); Serial.println(OCR1A);
	} else {
		Serial.println("Disabled");
	}
}


ISR(TIMER1_COMPA_vect) {
	output = !output;
	digitalWrite(pin, output);
}

void servoSetMicros(unsigned int micros) {
	// timer is on clock/8
	// clock is 12.5MHz (do know why, but it's what was measured with oscilloscope)
	// thus, 0.08 µs per tick
	// => number of ticks = (micros x 12.5) / 8 = micros x 25 / 16
	OCR1A = (micros >> 4) * 25;
}

void servoEnable(boolean _enabled) {
	enabled = _enabled;
	// TODO : enable/disable interrupt
	cli();
	if (enabled) {
		TCCR1B = 0x0A; // CTC and Clk/8
		TIMSK1 = 0x02;
	} else {
		TCCR1B = 0x08; // prescale 0 = disable timer
		TIMSK1 = 0x00;
		digitalWrite(pin, 0);
	}
	sei();
}
