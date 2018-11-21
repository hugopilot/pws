#ifndef GPIO_HANDLER_H
#define GPIO_HANDLER_H

// All macro's
#define INPUT false
#define OUTPUT true

class GPIO{
public:
	// This constructor uses GPIO4 as default
	GPIO();
	
	// This constuctor will initalize a GPIO pin
	GPIO(int _PIN); 
	void set_pin_mode(bool IsOutput);
	void write(bool turnOn);
	bool read();
	void export_gpio();
	void import_gpio();
	bool get_state();
	
private:
	int PIN_NUMBER;
	bool DIR_IS_OUT;
};

#endif
