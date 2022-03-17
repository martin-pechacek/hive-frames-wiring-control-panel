#ifndef abstract_h
#define abstract_h
#endif

#include "Motor.h"
#include "Button.h"
#include "ToggleStick.h"
#include "ControlPanel.h"

// Control Elements (Buttons, Toggle Stick)
Button forwardBtn(3, ButtonType::FORWARD);
Button backwardBtn(4, ButtonType::BACKWARD);
Button pauseBtn(5, ButtonType::PAUSE);
ToggleStick toggleStick(6);

// Control Panel
Button buttons[] = {forwardBtn, backwardBtn, pauseBtn};
ControlPanel controlPanel(buttons);

// Stepper Motor
const int stepsPerRevolution = 3600;
Motor motor(stepsPerRevolution, 9, 10, &pauseBtn);

void setup() {
  // Serial set up. Uncomment if Serial monitor needed
  // Serial.begin(9600);
}

void loop() {

  if(toggleStick.isActive()) {
    motor.setMode(MotorMode::AUTOMATIC);
  } else {
    motor.setMode(MotorMode::MANUAL);
  }
  
  Button* pressedBtn = controlPanel.getPressedBtn();

  // if none button is pressed go back to the beginning of a loop
  if(!pressedBtn) return;
  
  switch(pressedBtn->getType()) {
    case ButtonType::FORWARD:
      motor.moveForward();
      break;
    case ButtonType::BACKWARD:
      motor.moveBackward();
      break;
    default:
      break;
  }
}
