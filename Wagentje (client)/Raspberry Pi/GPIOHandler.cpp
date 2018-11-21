#include "GPIOHandler.h"
#include <string>
#include <sstream>
#include <fstream>

const std::string UNEXPORT_CMD = "/sys/class/gpio/unexport";
const std::string DIRSET_1 = "/sys/class/gpio/gpio";
const std::string DIRSET_2 = "/direction";
const std::string VARSET_1 ="/sys/class/gpio/gpio";
const std::string VARSET_2 = "/value";


GPIO::GPIO(int _PIN){
	this->PIN_NUMBER = _PIN;
}
void GPIO::export_gpio(){
	std::string EXPORT_CMD = "/sys/class/gpio/export";
	std::ofstream exportgpio(EXPORT_CMD.c_str());
	//if(exportgpio < 0){
		// Throw exception
	//}
	exportgpio << this->PIN_NUMBER;
	exportgpio.close();
}
