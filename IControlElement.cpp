#include "Arduino.h"
#include "IControlElement.h"

IControlElement::IControlElement(int pin) {
  pinMode(pin, INPUT_PULLUP);
  this->_pin = pin;
}

bool IControlElement::isActive() {
  int sensorVal = digitalRead(getPin());
  // there is a reverse logic therefore 
  // LOW status means button is pressed
  if(sensorVal == LOW) {
    return true;
  }
  return false;
}

int IControlElement::getPin() {
  return _pin;
}
