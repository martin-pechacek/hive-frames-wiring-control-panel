/*
 * Motor.cpp - Motor library for Wiring/Arduino - Version 1.1.0
 *
 * Original library        (0.1)   by Tom Igoe.
 * Two-wire modifications  (0.2)   by Sebastian Gassner
 * Combination version     (0.3)   by Tom Igoe and David Mellis
 * Bug fix for four-wire   (0.4)   by Tom Igoe, bug fix from Noah Shibley
 * High-speed stepping mod         by Eugene Kozlenko
 * Timer rollover fix              by Eugene Kozlenko
 * Five phase five wire    (1.1.0) by Ryan Orendorff
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *
 * Drives a unipolar, bipolar, or five phase Motor motor.
 *
 * When wiring multiple Motor motors to a microcontroller, you quickly run
 * out of output pins, with each motor requiring 4 connections.
 *
 * By making use of the fact that at any time two of the four motor coils are
 * the inverse of the other two, the number of control connections can be
 * reduced from 4 to 2 for the unipolar and bipolar motors.
 *
 * A slightly modified circuit around a Darlington transistor array or an
 * L293 H-bridge connects to only 2 microcontroller pins, inverts the signals
 * received, and delivers the 4 (2 plus 2 inverted ones) output signals
 * required for driving a Motor motor. Similarly the Arduino motor shield's
 * 2 direction pins may be used.
 *
 * The sequence of control signals for 5 phase, 5 control wires is as follows:
 *
 * Step C0 C1 C2 C3 C4
 *    1  0  1  1  0  1
 *    2  0  1  0  0  1
 *    3  0  1  0  1  1
 *    4  0  1  0  1  0
 *    5  1  1  0  1  0
 *    6  1  0  0  1  0
 *    7  1  0  1  1  0
 *    8  1  0  1  0  0
 *    9  1  0  1  0  1
 *   10  0  0  1  0  1
 *
 * The sequence of control signals for 4 control wires is as follows:
 *
 * Step C0 C1 C2 C3
 *    1  1  0  1  0
 *    2  0  1  1  0
 *    3  0  1  0  1
 *    4  1  0  0  1
 *
 * The sequence of control signals for 2 control wires is as follows
 * (columns C1 and C2 from above):
 *
 * Step C0 C1
 *    1  0  1
 *    2  1  1
 *    3  1  0
 *    4  0  0
 *
 * The circuits can be found at
 *
 * https://docs.arduino.cc/learn/electronics/Motor-motors#circuit
 */

#include "Arduino.h"
#include "Motor.h"
#include "Button.h"
/*
 * two-wire constructor.
 * Sets which wires should control the motor.
 */
Motor::Motor(int number_of_steps, int motor_pin_1, int motor_pin_2, Button* pauseBtn)
{
  this->step_number = 0;    // which step the motor is on
  this->direction = 0;      // motor direction
  this->last_step_time = 0; // timestamp in us of the last step taken
  this->number_of_steps = number_of_steps; // total number of steps for this motor

  // Arduino pins for the motor control connection:
  this->motor_pin_1 = motor_pin_1;
  this->motor_pin_2 = motor_pin_2;

  //set default speed
  setSpeed(SPEED);

  // set pause button
  this->pauseBtn = pauseBtn;

  // setup the pins on the microcontroller:
  pinMode(this->motor_pin_1, OUTPUT);
  pinMode(this->motor_pin_2, OUTPUT);
}

void Motor::moveForward() {
  step(getStepsByMode());
}

void Motor::moveBackward() {
  step(-getStepsByMode());
}

void Motor::setMode(MotorMode mode) {
  this->mode = mode;
}

int Motor::getStepsByMode() {
  switch(this->mode) {
    case AUTOMATIC:
      return AUTOMATIC_CYCLE_STEPS;
      break;
    case MANUAL:
      return MANUAL_CYCLE_STEPS;
      break;
    default:
      return 0;
      break;
  }
}

/*
 * Sets the speed in revs per minute
 */
void Motor::setSpeed(long whatSpeed)
{
  this->step_delay = 60L * 1000L * 1000L / this->number_of_steps / whatSpeed;
}

/*
 * Moves the motor steps_to_move steps.  If the number is negative,
 * the motor moves in the reverse direction.
 */
void Motor::step(int steps_to_move)
{  
  int steps_left = abs(steps_to_move);  // how many steps to take

  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0) { this->direction = 1; }
  if (steps_to_move < 0) { this->direction = 0; }


  // decrement the number of steps, moving one step each time:
  while (steps_left > 0)
  {
    unsigned long now = micros();
    // move only if the appropriate delay has passed:
    if (now - this->last_step_time >= this->step_delay)
    {
      // get the timeStamp of when you stepped:
      this->last_step_time = now;
      // increment or decrement the step number,
      // depending on direction:
      if (this->direction == 1)
      {
        this->step_number++;
        if (this->step_number == this->number_of_steps) {
          this->step_number = 0;
        }
      }
      else
      {
        if (this->step_number == 0) {
          this->step_number = this->number_of_steps;
        }
        this->step_number--;
      }
      // decrement the steps left:
      steps_left--;
      // step the motor to step number 0, 1, ..., {3 or 10}
     bool continue_loop = stepMotor(this->step_number % 4);

     if(!continue_loop) break;
    }
  }
}

/*
 * Moves the motor forward or backwards.
 */
bool Motor::stepMotor(int thisStep)
{
  switch (thisStep) {    
    case 0:  // 01
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_2, HIGH);
    break;
    case 1:  // 11
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_2, HIGH);
    break;
    case 2:  // 10
      digitalWrite(motor_pin_1, HIGH);
      digitalWrite(motor_pin_2, LOW);
    break;
    case 3:  // 00
      digitalWrite(motor_pin_1, LOW);
      digitalWrite(motor_pin_2, LOW);
    break;
  }
  return !pause();
}

bool Motor::pause() {
  if(this->pauseBtn->isActive()) {
    return true;
  }
  return false;
}

/*
  version() returns the version of the library:
*/
int Motor::version(void)
{
  return 5;
}
