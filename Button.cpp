#include "Arduino.h"
#include "Button.h"

Button::Button(int pin, ButtonType type): IControlElement(pin) {
  _type = type;
}

ButtonType Button::getType() {
  return _type;
}
