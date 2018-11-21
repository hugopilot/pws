/* Library written by Hugo Woesthuis
 * Licensed under GPLv3
 */

#pragma once
#include <Arduino.h>
#include "Motor.h"


// Constructor
void Motor::Init(uint8_t _ENABLE, uint8_t _CTRL1, uint8_t _CTRL2){
  ENABLE = _ENABLE;
  CTRL1 = _CTRL1;
  CTRL2 = _CTRL2;
  CURRENT_DIR = STOP;
  CURRENT_SPD = 0;
  pinMode(ENABLE, OUTPUT);
  pinMode(CTRL1, OUTPUT);
  pinMode(CTRL2, OUTPUT);
  analogWrite(ENABLE, 0);
}

// Change direction
void Motor::change_dir(Direction dir){
    switch(dir){
      case FORWARD:
      digitalWrite(CTRL1, HIGH);
      digitalWrite(CTRL2, LOW);
      CURRENT_DIR = FORWARD;
      break;
      case BACKWARD:
      digitalWrite(CTRL1, LOW);
      digitalWrite(CTRL2, HIGH);
      CURRENT_DIR = BACKWARD;
      break;
      case STOP:
      digitalWrite(CTRL1, LOW);
      digitalWrite(CTRL2, LOW);
      CURRENT_DIR = STOP;
    }
}

// Disable control
void Motor::disable(){
  analogWrite(ENABLE, 0);
  ENABLED = false;
}

// Enable control
void Motor::enable(){
  analogWrite(ENABLE, CURRENT_SPD);
  ENABLED = true;
}

void Motor::set_speed(long NEW_SPD){
  CURRENT_SPD = NEW_SPD;
  analogWrite(ENABLE, CURRENT_SPD);
}

bool Motor::get_enabled(){
  return ENABLED;
}
long Motor::get_speed(){
  return CURRENT_SPD;
}
Direction Motor::get_dir(){
  return CURRENT_DIR;
}

