#include "Arduino.h"
#include "Button.h"
#include "ControlPanel.h"

ControlPanel::ControlPanel(Button btns[]) {
  _buttons = btns;
}

ControlPanel::~ControlPanel(){
  delete [] _buttons;
};

Button* ControlPanel::getPressedBtn() {
  for(int i=0; i < ARRAY_SIZE; i++) {
    Button* btn = &_buttons[i];
    if(btn->isActive()) {
      return btn;
    }
  }
  return NULL;
}
