#include "task_led.h"

void TaskLed::init() {
	led_.init(GPIOC, GPIO_PIN_13);
	add_task_event(500, this->blink_led_gpioc_13, EVENT_MODE_PERIOD, &led_);
}

void TaskLed::main_handler() {}

void TaskLed::blink_led_gpioc_13(void *argument) {
	static_cast<Led *>(argument)->toggle();
}

extern "C" void task_led_handler(void *argument) {
	(void)argument;
	TaskLed task;
	task.init();
	task.loop();
}
