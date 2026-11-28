#ifndef __TASK_SYS__
#define __TASK_SYS__

#include "task.h"

class TaskSys : public Task {
public:
	void init();
	void main_handler() override;
	void on_event(uint32_t id, void *argument) override;

	// Getter
	bool fw_update_requested();

private:
	uint32_t required_ready_mask_		   = 0;
	volatile uint32_t fw_update_requested_ = 0;
	volatile uint32_t ready_mask_		   = 0;

	void build_required_ready_mask();
	int request_fw_update();
	int set_update_request_in_eeprom();
};

extern "C" void task_sys_handler(void *argument);

#endif	  //__TASK_SYS__
