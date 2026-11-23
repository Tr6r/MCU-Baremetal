#ifndef __TASK__
#define __TASK__

extern "C" {
#include "baros.h"
}
#include "timer.h"

#define MAX_EVENTS 8

typedef enum {
	EVENT_MODE_ONCE_TIME,
	EVENT_MODE_PERIOD
} event_mode_e;

typedef struct {
	Timer timer;
	void (*callback)(void *argument);
	void *argument;
	event_mode_e mode;
} task_event_t;

class Task {
public:
	Task();
	~Task();
	virtual void main_handler() = 0;
	void add_task_event(uint32_t ms, void (*callback)(void *argument), event_mode_e mode, void *argument = nullptr);
	void loop();

protected:
	void run_events();
	task_event_t events[MAX_EVENTS];
	uint32_t event_count_ = 0;
};

#endif	  //__TASK__
