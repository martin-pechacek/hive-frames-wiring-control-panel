#ifndef ToggleStick_h
#define ToggleStick_h

#include "Arduino.h"
#include "IControlElement.h"

class ToggleStick : public IControlElement {
  public:
    ToggleStick(int pin);
};

#endif
