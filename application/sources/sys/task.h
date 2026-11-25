#ifndef __TASK__
#define __TASK__

extern "C" {
#include "baros.h"
}
#include "timer.h"

#define MAX_EVENTS 8
#define MAX_MSG	   8

typedef enum {
	EVENT_MODE_ONCE_TIME,
	EVENT_MODE_PERIOD
} event_mode_e;

typedef struct {
	uint32_t id;
	void *argument;
} event_msg_t;

typedef struct {
	Timer timer;
	event_msg_t msg;
	event_mode_e mode;
} task_event_t;

class Task {
public:
	Task();
	~Task();
	virtual void main_handler() = 0;
	virtual void on_event(uint32_t id, void *argument) = 0;
	void add_task_event(uint32_t ms, event_msg_t msg, event_mode_e mode);
	void post(uint32_t id, void *argument = nullptr);
	void loop();

protected:
	void run_events();
	void drain_inbox();
	task_event_t events[MAX_EVENTS];
	uint32_t event_count_ = 0;

	event_msg_t inbox_[MAX_MSG];
	volatile uint8_t head_ = 0;
	volatile uint8_t tail_ = 0;
};

#endif	  //__TASK__
