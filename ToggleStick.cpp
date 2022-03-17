#include "Arduino.h"
#include "ToggleStick.h"

ToggleStick::ToggleStick(int pin): IControlElement(pin) {
  pinMode(pin, INPUT_PULLUP);
}
