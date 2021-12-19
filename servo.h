#ifndef HAVE_SERVO_H
#define HAVE_SERVO_H

#include <Arduino.h>

void servoSetup(byte pin);

void servoStatus();

void servoSetMicros(unsigned int micros);

void servoEnable(boolean enabled);

#endif // HAVE_SERVO_H
