#ifndef ControlPanel_h
#define ControlPanel_h

#include "Arduino.h"
#include "Button.h"

class ControlPanel {
  private:
    static const int ARRAY_SIZE = 3;
    Button *_buttons;
  public:
    // initialize use a pointer to an array
    ControlPanel(Button *btns);
    ~ControlPanel();
    Button* getPressedBtn();
};

#endif
