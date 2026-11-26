extern "C" {
#include "baros.h"
}

#include "task.h"

Task::Task() {
	event_count_ = 0;
	g_baros.bind_self(&g_baros, this);
}

Task::~Task() {}

void Task::add_task_event(uint32_t id, uint32_t ms, event_mode_e mode, void *argument) {
	if (event_count_ >= MAX_EVENTS) {
		return;
	}
	task_event_t &e = events[event_count_++];
	e.timer.set_period(ms);
	e.id	   = id;
	e.mode	   = mode;
	e.argument = argument;
}

void Task::run_events() {
	uint32_t i = 0;
	while (i < event_count_) {
		task_event_t &e = events[i];
		if (e.timer.is_due()) {
			on_event(e.id, e.argument);
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

void post_task_event(uint32_t thread_id, uint32_t id, uint32_t ms, event_mode_e mode, void *argument) {
	Task *t = static_cast<Task *>(g_baros.obj_of(&g_baros, thread_id));
	if (t == nullptr) {
		return;
	}
	t->add_task_event(id, ms, mode, argument);
}

extern "C" void __cxa_pure_virtual() {
	for (;;) {}
}
