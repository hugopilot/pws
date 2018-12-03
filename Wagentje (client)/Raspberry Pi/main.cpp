#include <iostream>

#include "host_handler.h"

// SEND ASCII
int main(int argc, char** argv){
	host_handler host("/home/pi/Desktop/pws");
	host.start("10.0.0.117", 2121);
}
