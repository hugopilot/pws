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
	ENABLE
};
class MotorController{
public:
	static void SendCommand(command _cmd, int address);
};
#endif
