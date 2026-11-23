#include "led.h"

void Led::init(GPIO_TypeDef *port, GPIO_Pin_t pin) {
	port_  = port;
	pin_   = pin;
	state_ = LED_STATE_OFF;
	GPIO_Init(port_, pin_, GPIO_MODE_OUTPUT_10MHz, GPIO_CNF_OUTPUT_PP);
	GPIO_WritePin(port_, pin_, GPIO_LOW);
}

void Led::on() {
	state_ = LED_STATE_ON;
	GPIO_WritePin(port_, pin_, GPIO_HIGH);
}

void Led::off() {
	state_ = LED_STATE_OFF;
	GPIO_WritePin(port_, pin_, GPIO_LOW);
}

void Led::toggle() {
	state_ = (state_ == LED_STATE_ON) ? LED_STATE_OFF : LED_STATE_ON;
	GPIO_Write_Toggle(port_, pin_);
}
