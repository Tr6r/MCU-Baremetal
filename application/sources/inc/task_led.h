#ifndef __TASK_LED__
#define __TASK_LED__

#include "task.h"
#include "led.h"

class TaskLed : public Task {
public:
	void init();
	void main_handler() override;

private:
	Led led_;

	static void blink_led_gpioc_13(void *argument);
};

extern "C" void task_led_handler(void *argument);

#endif	  //__TASK_LED__