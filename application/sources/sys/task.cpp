extern "C" {
#include "baros.h"
}

#include "task.h"

Task::Task() {
	event_count_ = 0;
}

Task::~Task() {}

void Task::add_task_event(uint32_t ms, void (*callback)(void *argument), event_mode_e mode, void *argument) {
	if (event_count_ >= MAX_EVENTS) {
		return;
	}
	task_event_t &e = events[event_count_++];
	e.timer.set_period(ms);
	e.callback = callback;
	e.argument = argument;
	e.mode	   = mode;
}

void Task::run_events() {
	uint32_t i = 0;
	while (i < event_count_) {
		task_event_t &e = events[i];
		if (e.timer.is_due()) {
			e.callback(e.argument);
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
		run_events();
		main_handler();
		baros_delay(1);
	}
}

extern "C" void __cxa_pure_virtual() {
	for (;;) {
	}
}