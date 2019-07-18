#include "Note.h"
#include <Arduino.h>

// Overloaded constructers for including next value or not
Note::Note(int value, int duration) {
  _value = value;
  _duration = duration;
  _next = NULL;
}

Note::Note(int value, int duration, Note* next) {
  _value = value;
  _duration = duration;
  _next = next;
}

void Note::printOut() {
  char output[40];
  sprintf(output, "Value = %d \t Duration = %d", _value, _duration);
  Serial.println(output);
  }

bool Note::hasNext() {
  return _next != NULL;
}

Note* Note::getNext() {
  return _next;
}

void Note::setNext(Note* next) {
  _next = next;
}

int Note::getValue() {
  return _value;
}

int Note::getDuration() {
  return _duration;
}

void Note::setDuration(int duration) {
  _duration = duration;
}

void Note::setValue(int value) {
  _value = value;
}
