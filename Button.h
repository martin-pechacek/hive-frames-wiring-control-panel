#ifndef Button_h
#define Button_h

#include "Arduino.h"
#include "IControlElement.h"

enum ButtonType { FORWARD, BACKWARD, PAUSE };

class Button: public IControlElement {
  public:
    Button(int pin, ButtonType type);
    ~Button(){};
    ButtonType getType();
  private:
    ButtonType _type;
};

#endif
