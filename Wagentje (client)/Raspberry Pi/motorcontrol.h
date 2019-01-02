/* Written by Hugo Woesthuis
 * Licensed under GPLv3
 */

#include <iostream>
#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H
#define MOTORCOMMAND "python i2c_comm.py"
#define STOPN "48"
#define SFSN "49"
#define TLN "50"
#define TRN "51"
#define GSN "52"
#define GBN "53"
#define STLN "55"
#define STRN "56"
#define DN "57"
#define EN "58"

int exec_py_script(std::string path);
void reset_mc();
// Commands to be given to the handler
enum command{
	STOP,
	SET_FULL_SPEED,
	TURN_LEFT,
	TURN_RIGHT,
	GO_STRAIGHT,
	GO_BACK,
	SHARP_TURN_LEFT,
	SHARP_TURN_RIGHT,
	DISABLE,
	ENABLE,
	IGNORE
};
class MotorController{
public:
	static void SendCommand(command _cmd, int address);
	static command ConvertStringToCommand(std::string _string);
};
#endif
