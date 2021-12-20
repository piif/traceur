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

//	//	for testing purpose : OC1A and OC1B outputs
//	pinMode(12, OUTPUT);
//	pinMode(13, OUTPUT);

	// run counter from 0 to OCR1A , switch on output on OCR1A value, and switch down on OCR1B value
	// no OC1x output
	// WGM mode CTC with TOP=OCR1A = 100 ?
	// prescale /8 = 010
	// interrupt on compare match A and B
	// OCR1A value = ~25ms , OCR1B value = required µs

	cli();

	// prepare timer
	TCCR1A = 0x00; // 00 01 -- 00 , st to 0x50 to test OC1B and OC1A values
	TCCR1B = 0x00; // init disabled, else 0x0A : 00 - 01 001
	TIMSK1 = 0x00; // init disabled, else 0x06 : ----- 100
	OCR1A = 40625; // 26ms
	sei();
}

void servoStatus() {
	Serial.print("Pin   : "); Serial.println(pin);
	if (enabled) {
		Serial.print("Micros: "); Serial.print(OCR1B);
		Serial.print(" / "); Serial.println(OCR1A);
	} else {
		Serial.println("Disabled");
	}
}


ISR(TIMER1_COMPB_vect) {
	digitalWrite(pin, LOW);
}
ISR(TIMER1_COMPA_vect) {
	digitalWrite(pin, HIGH);
}

void servoSetMicros(unsigned int micros) {
	if (micros == 0) {
		servoEnable(0);
	} else {
		servoEnable(1);
		// timer is on clock/8
		// clock is 12.5MHz (do know why, but it's what was measured with oscilloscope)
		// thus, 0.08 µs per tick
		// => number of ticks = (micros x 12.5) / 8 = micros x 25 / 16
		OCR1B = (micros >> 4) * 25;
	}
}

void servoEnable(boolean _enabled) {
	if (enabled == _enabled) {
		return;
	}
	enabled = _enabled;
	cli();
	if (enabled) {
		TCCR1B = 0x0A; // CTC and Clk/8
		TIMSK1 = 0x06;
	} else {
		TCCR1B = 0x08; // prescale 0 = disable timer
		TIMSK1 = 0x00;
		digitalWrite(pin, 0);
	}
	sei();
}
