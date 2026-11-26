
#include "task_led.h"

static void blink_led_gpioc_13(Led &led);

void TaskLed::init() {
	led_.init(GPIOC, GPIO_PIN_13);
	g_baros.bind_self(&g_baros, this);
	add_task_event(LED_BLINK, LED_BLINK_TIME, EVENT_MODE_PERIOD);
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

static void blink_led_gpioc_13(Led &led) {
	led.toggle();
}

extern "C" void task_led_handler(void *argument) {
	(void)argument;
	TaskLed task;
	task.init();
	task.loop();
}
