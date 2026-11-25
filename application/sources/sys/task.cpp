extern "C" {
#include "baros.h"
}

#include "task.h"

Task::Task() {
	event_count_ = 0;
	head_		 = 0;
	tail_		 = 0;
}

Task::~Task() {}

void Task::add_task_event(uint32_t ms, event_msg_t msg, event_mode_e mode) {
	if (event_count_ >= MAX_EVENTS) {
		return;
	}
	task_event_t &e = events[event_count_++];
	e.timer.set_period(ms);
	e.msg  = msg;
	e.mode = mode;
}

void Task::post(uint32_t id, void *argument) {
	baros_enter_critical();
	uint8_t next = (uint8_t)((head_ + 1) % MAX_MSG);
	if (next != tail_) {
		inbox_[head_].id	   = id;
		inbox_[head_].argument = argument;
		head_				   = next;
	}
	baros_exit_critical();
}

void Task::drain_inbox() {
	while (tail_ != head_) {
		event_msg_t m = inbox_[tail_];
		tail_		  = (uint8_t)((tail_ + 1) % MAX_MSG);
		on_event(m.id, m.argument);
	}
}

void Task::run_events() {
	uint32_t i = 0;
	while (i < event_count_) {
		task_event_t &e = events[i];
		if (e.timer.is_due()) {
			on_event(e.msg.id, e.msg.argument);
			if (e.mode == EVENT_MODE_ONCE_TIME) {
				for (uint32_t j = i; j + 1 < event_count_; j++) {
					events[j] = events[j + 1];
				}
				event_count_--;
				continue;
			}
		}
		i++;
	}
}

void Task::loop() {
	for (;;) {
		drain_inbox();
		run_events();
		main_handler();
		baros_delay(1);
	}
}

extern "C" void __cxa_pure_virtual() {
	for (;;) {
	}
}
