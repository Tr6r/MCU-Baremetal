#ifndef __LED__
#define __LED__

extern "C" {
#include "rcc.h"
#include "gpio.h"
}

typedef enum {
	LED_STATE_ON,
	LED_STATE_OFF,
} LedState;

class Led {
public:
	void init(GPIO_TypeDef *port, GPIO_Pin_t pin);
	void on();
	void off();
	void toggle();

private:
	GPIO_TypeDef *port_;
	GPIO_Pin_t pin_;
	LedState state_;
};

#endif	  // __LED__