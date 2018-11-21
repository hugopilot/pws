/* Library written by Hugo Woesthuis
 * Licensed under GPLv3
 */
 
#pragma once

#ifndef MOTOR_H
#define MOTOR_H

enum Direction{
  FORWARD,
  BACKWARD,
  STOP
};
class Motor{
  public:
  //SETTERS
  // Constructor
  void Init(uint8_t _ENABLE, uint8_t _CTRL1, uint8_t _CTRL2);

  // Change state
  void change_dir(Direction dir);

  // TODO: Change speed
  // void change_motor_speed(long speed);

  // Disable control
  void disable();

  // Enable control
  void enable();

  // Set speed(PWM)
  void set_speed(long NEW_SPD);


  //GETTERS
  // Get current direction
  Direction get_dir();
  bool get_enabled();
  long get_speed();

  
  private:
  Direction CURRENT_DIR;
  long CURRENT_SPD;
  uint8_t ENABLE;
  uint8_t CTRL1;
  uint8_t CTRL2;
  bool ENABLED = false;
};
#endif

