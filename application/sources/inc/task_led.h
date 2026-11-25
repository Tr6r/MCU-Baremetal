#ifndef __TASK_LED__
#define __TASK_LED__

#include "task.h"
#include "led.h"

#define LED_BLINK_TIME 200

typedef enum {
	LED_BLINK = 0,
	LED_EVENT_COUNT
} led_event_id;

class TaskLed : public Task {
public:
	void init();
	void main_handler() override;
	void on_event(uint32_t id, void *argument) override;

private:
	Led led_;
};

extern "C" void task_led_handler(void *argument);

#endif	  //__TASK_LED__