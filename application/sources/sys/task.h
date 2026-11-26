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
	uint32_t id;
	Timer timer;
	void *argument;
	event_mode_e mode;
} task_event_t;

class Task {
public:
	Task();
	~Task();
	virtual void main_handler()						   = 0;
	virtual void on_event(uint32_t id, void *argument) = 0;
	void add_task_event(uint32_t id, uint32_t ms, event_mode_e mode, void *argument = nullptr);
	void loop();

protected:
	void run_events();
	task_event_t events[MAX_EVENTS];
	uint32_t event_count_ = 0;
};

void post_task_event(uint32_t thread_id, uint32_t id, uint32_t ms, event_mode_e mode, void *argument);

#endif	  //__TASK__
