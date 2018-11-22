#include <iostream>
#include <thread>
#include <chrono>
#include "host_handler.h"

void sleep(long _milliseconds){
	std::this_thread::sleep_for(std::chrono::milliseconds(_milliseconds));
}

// SEND ASCII
int main(int argc, char** argv){
	tcp_client cl("localhost", 5000);
	std::cout<<"tcp settings obtained!\n";
	if(!cl.init()){
		return 66;
	}
	cl.send_data("lalalala");
	std::cout << cl.receive_data(16) << "\n\n";
	std::cout<<"Hallo\n";
	MotorController::SendCommand(ENABLE, 4);
	MotorController::SendCommand(STOP,4);
	MotorController::SendCommand(GO_STRAIGHT, 4);
	sleep(1000);
	MotorController::SendCommand(STOP, 4);
	return 0;
}
