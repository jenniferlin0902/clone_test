#include "gpio.h"

// address 
#define GPIO_SET0 0x2020001C
#define GPIO_SET1 0x20200020
#define GPIO_CLR0 0x20200028
#define GPIO_CLR1 0x2020002C
#define GPIO_LVL0 0x20200034
#define GPIO_FSEL_BASE 0x20200000
#define GPIO_FSEL0 GPIO_FSEL_BASE
#define GPIO_FSEL_PIN_SIZE 0x3 


void gpio_init(void) {
}


void gpio_set_function(unsigned int pin, unsigned int function) {
   // first find the GPIO pin FSEL address 
	volatile unsigned int* regAddr;
	unsigned int offset = (pin % 10) * GPIO_FSEL_PIN_SIZE;
	unsigned int group = pin / 10;
	regAddr = (unsigned int*)(GPIO_FSEL_BASE);
	regAddr += group;

	// first read the regAddr 
	unsigned int oldVal = *regAddr; 
	unsigned int newVal = (oldVal & (~(0b111 << offset))) | (function << offset); 
	// write to regAddr 
	*regAddr = newVal;
}

unsigned int gpio_get_function(unsigned int pin) {
      // first find the GPIO pin FSEL address 
	volatile unsigned int* regAddr;
	unsigned int offset = (pin % 10) * GPIO_FSEL_PIN_SIZE;
	unsigned int group = pin / 10;
	regAddr = (unsigned int*)(GPIO_FSEL_BASE);
	regAddr += group;
	// first read the regAddr 
	unsigned int function = (*regAddr & (0b111 << offset)) >> offset;
	return function;
}

void gpio_set_input(unsigned int pin) {
    gpio_set_function(pin, GPIO_FUNC_INPUT);
}

void gpio_set_output(unsigned int pin) {
    gpio_set_function(pin, GPIO_FUNC_OUTPUT);
}

void gpio_write(unsigned int pin, unsigned int value) {
	// assume value is 1 or 0
	// first find GPIO SET address 
	volatile unsigned int* regAddr = 0;
	unsigned int offset = pin % 32;
	unsigned int group = pin / 32;
	if (value == 1){
		regAddr = (unsigned int*)(GPIO_SET0);
	} else if (value == 0) {
		regAddr = (unsigned int*)(GPIO_CLR0);
	}
	regAddr += group;

	//unsigned int oldVal = *regAddr;
	//unsigned int newVal = (oldVal & ~(0x1<<offset)) | (0b1<<offset);
	*regAddr = (0b1<<offset);
}

unsigned int gpio_read(unsigned int pin) {
	volatile unsigned int* regAddr;
	unsigned int offset = pin % 32;
	unsigned int group = pin / 32;
	regAddr = (unsigned int*)(GPIO_LVL0);
	regAddr += group;

	unsigned int val = ((*regAddr) >> offset) & 0b1;
	return val;
}
