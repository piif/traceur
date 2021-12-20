#ifndef HAVE_PINS_H
#define HAVE_PINS_H

// FROM arduino pins.h , variant "standard" and not "sanguino" (D24 to D31 inverted)
// D0  = PB0
// D1  = PB1
// D2  = PB2
// D3  = PB3 , OC0A
// D4  = PB4 , OC0B
// D5  = PB5 , MOSI
// D6  = PB6 , MISO
// D7  = PB7 , SCK

// D8  = PD0
// D9  = PD1
// D10 = PD2
// D11 = PD3
// D12 = PD4 , OC1B
// D13 = PD5 , 0C1A
// D14 = PD6 , OC2B
// D15 = PD7 , OC2A

// D16 = PC0
// D17 = PC1
// D18 = PC2
// D19 = PC3
// D20 = PC4
// D21 = PC5
// D22 = PC6
// D23 = PC7

// D24 = PA0
// D25 = PA1
// D26 = PA2
// D27 = PA3
// D28 = PA4
// D29 = PA5
// D30 = PA6
// D31 = PA7

// FROM https://github.com/kliment/Sprinter/blob/master/Sprinter/pins.h#L301
// + adaptations because this code is based on "sanguino" variant

// x axis
#define X_STEP_PIN 15   // PD7 , OC2A
#define X_DIR_PIN 18    // PC2
#define X_ENABLE_PIN 31 // PA7
#define X_MIN_PIN 20    // PC4 = PCINT20
#define X_MIN_INTR  PCINT20
#define X_MIN_PCMSK PCMSK2
#define X_MIN_PCICR PCIE2
#define X_MIN_PCINT_vect PCINT2_vect

// y axes
#define Y_STEP_PIN 23   // PC7
#define Y_DIR_PIN 22    // PC6
#define Y_ENABLE_PIN 31 // PA7
#define Y_MIN_PIN 30    // PA6 = PCINT6
#define Y_MIN_INTR  PCINT6
#define Y_MIN_PCMSK PCMSK0
#define Y_MIN_PCICR PCIE0
#define Y_MIN_PCINT_vect PCINT0_vect

// z axes
#define Z_STEP_PIN 28   // PA4
#define Z_DIR_PIN 27    // PA3
#define Z_ENABLE_PIN 26 // PA2
#define Z_MIN_PIN 25    // PA1 = PCINT1
#define Z_MIN_INTR  PCINT6
#define Z_MIN_PCMSK PCMSK0
#define Z_MIN_PCICR PCIE0
#define Z_MIN_PCINT_vect PCINT0_vect

//extruder pins
#define E_STEP_PIN 12   // PD4 , OC1B
#define E_DIR_PIN 17    // PC1
#define E_ENABLE_PIN 3  // PB3 , OC0A

#define DEBUG_PIN 0

// LOW should open mofset linked to FAN connector
#define FAN_PIN 2 // PB2

// 5 lines of 3 pins (2 for bottom one) at bottom left corner
// see http://reprap.org/wiki/Monotronics#Pinout_Maps
// pinout :
// GND +5V D26/A5
// GND +5V D1
// GND +5V ARef
// GND +5V D4
// GND D31/A0
#define USER_0 29  // PA5
#define USER_1 1   // PB1
// no USER_2 pin
#define USER_3 4   // PB4 , OC0B
#define USER_4 24  // PA0

#define SERVO USER_3

#endif // HAVE_PINS_H
