#ifndef ControlElement_h
#define ControlElement_h

#include "Arduino.h"

class IControlElement {
  public:
    bool isActive();
    int getPin();
  protected:
    IControlElement(int pin);
    ~IControlElement(){};
  private:
    int _pin;
};

#endif
