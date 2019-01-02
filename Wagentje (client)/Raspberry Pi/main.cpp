/* Written by Hugo Woesthuis
 * Licensed under GPLv3
 */

#include <iostream>

#include "host_handler.h"

void on_exit(){
	std::cout<<"stopped!";
}

// SEND ASCII
int main(int argc, char** argv){
	if(argc != 3){return 160;}
	std::atexit(on_exit);
	host_handler host("/home/pi/Desktop/pws");
	host.start(argv[1], std::stoi(argv[2]));
	
	
	return 0;
}
