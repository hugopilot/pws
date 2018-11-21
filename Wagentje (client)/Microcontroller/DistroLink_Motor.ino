/* Written by Hugo Woesthuis and Mike Krop
 * Licensed under GPLv3, see LICENSE file for more info.
 */

#include <Wire.h>
#include "Motor.h"

// Define adresses and pins
#define I2C_ADDRESS 0x04
#define M1_ENABLE 5
#define M2_ENABLE 6
#define M1_CTRL_1 8
#define M1_CTRL_2 9
#define M2_CTRL_1 10
#define M2_CTRL_2 11

// Initalize motor classes
Motor m1;
Motor m2;
void setup() {
  // put your setup code here, to run once:
  m1.Init(M1_ENABLE, M1_CTRL_1, M1_CTRL_2);
  m2.Init(M2_ENABLE, M2_CTRL_1, M2_CTRL_2);

  if(!m1.get_enabled()){
    m1.enable();
  }
  if(!m2.get_enabled()){
    m2.enable();
  }
  Wire.begin(I2C_ADDRESS);
  Serial.begin(9600);
  Serial.println("SETUP OK");
  Wire.onReceive(command_receive);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
}

/* Motor movement table:
 * Binary | Decimal | Action
 *    0000        0   Stop moving
 *    0001        1   Set full speed
 *    0010        2   Turn left (m1 off, m2 on)
 *    0011        3   Turn right (m1 on, m2 off)
 *    0100        4   Go staight
 *    0101        5   Go backwards
 *    0110        6   Set speed (next byte is speed, 8bit precision)
 *    0111        7   Sharp turn left (M1 backwards, M2 Forwards)
 *    1000        8   Sharp turn right (M1 forwards, m2 backwards)
 *    1001        9   Disable all motors.
 */
void command_receive(int byteCount){
  while(Wire.available() > 0){
    char cmd = Wire.read();
    Serial.println(cmd);
    switch(cmd){
      case '0':
      m1.change_dir(STOP);
      m2.change_dir(STOP);
      break;
      case '1': 
        m1.set_speed(255);
        m2.set_speed(255);
      case '2':
        m1.change_dir(STOP);
        m2.change_dir(FORWARD);
        break;
      case '3':
        m1.change_dir(FORWARD);
        m2.change_dir(STOP);
        break;
      case '4':
        m1.change_dir(FORWARD);
        m2.change_dir(FORWARD);
        Serial.println(m1.get_speed());
        break;
      case '5':
        m1.change_dir(BACKWARD);
        m2.change_dir(BACKWARD);
        break;
      case '6':
        if(Wire.available()>0){
          int SPD = Wire.read();
          m1.set_speed(SPD);
          m2.set_speed(SPD);
        }
        break;
      case '7':
        m1.change_dir(BACKWARD);
        m2.change_dir(FORWARD);
        break;
      case '8':
        m1.change_dir(FORWARD);
        m2.change_dir(BACKWARD);
        break;
      case '9':
        m1.disable();
        m2.disable();
        break;
      default:
        continue;        
    }
  }
}

