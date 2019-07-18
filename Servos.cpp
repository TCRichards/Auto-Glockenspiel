#include "Servos.h"
#include <Servo.h>
#include <Arduino.h>

Servos::Servos(int pin, int minAngle, int maxAngle) {
  servo.attach(pin);
  _pin = pin;
  _minAngle = minAngle;
  _maxAngle = maxAngle;
  servo.write(minAngle);  // Start in the closed position
}

void Servos::writeTo(int angle) {
  servo.write(angle);
  delay(30);
}

int Servos::getMinAngle() {
  return _minAngle;
}

int Servos::getMaxAngle() {
  return _maxAngle;
}

int Servos::getMidAngle() {
  return (_minAngle + _maxAngle)/2;
}

void Servos::drop() {
  servo.write(getMidAngle());
  delay(50);
  servo.write(_maxAngle);
  delay(50);
  servo.write(getMidAngle());
  delay(50);
  servo.write(_minAngle);
}
