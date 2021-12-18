#ifndef HAVE_PINS_H
#define HAVE_PINS_H

// FROM arduino pins.h
// D0  = PB0
// ..
// D7  = PB7
// D8  = PD0
// ..
// D15 = PD7
// D16 = PC0
// ..
// D23 = PC7
// D24 = PA7
// ..
// D31 = PA0

// FROM https://github.com/kliment/Sprinter/blob/master/Sprinter/pins.h#L301
// x axis
#define X_STEP_PIN 15
#define X_DIR_PIN 18
#define X_ENABLE_PIN 31 // doc says 24 !
#define X_MIN_PIN 20 // = PC4 = PCINT20
#define X_MIN_INTR  PCINT20
#define X_MIN_PCMSK PCMSK2
#define X_MIN_PCICR PCIE2
#define X_MIN_PCINT_vect PCINT2_vect

// y axes
#define Y_STEP_PIN 23
#define Y_DIR_PIN 22
#define Y_ENABLE_PIN 31 // doc says 24 !
#define Y_MIN_PIN 30 // PA1 = PCINT1
#define Y_MIN_INTR  PCINT1
#define Y_MIN_PCMSK PCMSK0
#define Y_MIN_PCICR PCIE0
#define Y_MIN_PCINT_vect PCINT0_vect

// z axes
#define Z_STEP_PIN 28
#define Z_DIR_PIN 27
#define Z_ENABLE_PIN 29
#define Z_MIN_PIN 25 // PA6 = PCINT6
#define Z_MIN_INTR  PCINT6
#define Z_MIN_PCMSK PCMSK0
#define Z_MIN_PCICR PCIE0
#define Z_MIN_PCINT_vect PCINT0_vect

//extruder pins
#define E_STEP_PIN 12
#define E_DIR_PIN 17
#define E_ENABLE_PIN 3 // PB3 =  PCINT11
#define E_MIN_INTR  PCINT11
#define E_MIN_PCMSK PCMSK1
#define E_MIN_PCICR PCIE1
#define E_MIN_PCINT_vect PCINT1_vect

#define DEBUG_PIN 0

// LOW should open mofset linked to FAN connector
#define FAN_PIN 2

// 5 lines of 3 pins (2 for bottom one) at bottom left corner
// see http://reprap.org/wiki/Monotronics#Pinout_Maps
// pinout :
// GND +5V D26/A5
// GND +5V D1
// GND +5V ARef
// GND +5V D4
// GND D31/A0
#define USER_1 26
#define USER_2 1
#define USER_3 4
#define USER_4 31

#endif // HAVE_PINS_H
