#include <iostream>

#include "host_handler.h"

// SEND ASCII
int main(int argc, char** argv){
	host_handler host("/home/pi/Desktop/pws");
	host.start("localhost", 5000);
}
