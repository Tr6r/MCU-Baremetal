#include "task_led.h"

static void blink_led_gpioc_13(	Led &led);

void TaskLed::init() {
	led_.init(GPIOC, GPIO_PIN_13);
	add_task_event(LED_BLINK_TIME, {LED_BLINK, nullptr}, EVENT_MODE_PERIOD);
}

void TaskLed::on_event(uint32_t id, void *argument) {
	switch (id) {
	case LED_BLINK: {
		blink_led_gpioc_13(led_);
		break;
	}
	default:
		break;
	}
}

void TaskLed::main_handler() {}

static void blink_led_gpioc_13(	Led &led) {
	led.toggle();
}

extern "C" void task_led_handler(void *argument) {
	(void)argument;
	TaskLed task;
	task.init();
	task.loop();
}
