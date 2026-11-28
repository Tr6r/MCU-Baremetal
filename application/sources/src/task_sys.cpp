extern "C" {
#include "eeproom.h"
#include "scb.h"
}

#include "task_sys.h"
#include "task_list.h"

extern "C" {
#include "scb.h"
}

static volatile uint32_t fw_update_requested = 0;
static volatile uint32_t ready_mask			 = 0;

int TaskSys::set_update_request_in_eeprom() {
	eeprom_data_t data;
	int rc = EEPROM_Read(&data);
	if (rc != HAL_OK) {
		return rc;
	}

	data.ota.update_request = true;
	return EEPROM_Write(&data);
}

int TaskSys::request_fw_update() {
	if (fw_update_requested_ != 0) {
		return HAL_OK;
	}

	int rc = set_update_request_in_eeprom();
	if (rc != HAL_OK) {
		return rc;
	}

	baros_enter_critical();
	ready_mask_			 = 0;
	fw_update_requested_ = 1;
	baros_exit_critical();

	return HAL_OK;
}

bool TaskSys::fw_update_requested() {
	return fw_update_requested_ != 0;
}

void TaskSys::init() {
	build_required_ready_mask();
}

void TaskSys::build_required_ready_mask() {
	required_ready_mask_ = 0;
	for (uint32_t i = 0; i < g_task_count; i++) {
		uint32_t id = g_task_list[i].thread_id;
		if (id < 32 && id != TASK_ID_SYS) {
			required_ready_mask_ |= 1U << id;
		}
	}
}

void TaskSys::main_handler() {
	if (fw_update_requested_ != 0 && (ready_mask & required_ready_mask_) == required_ready_mask_) {
		SCB_SystemReset();
	}
	baros_delay(999);
}

void TaskSys::on_event(uint32_t id, void *argument) {
	(void)id;
	(void)argument;
}

extern "C" void task_sys_handler(void *argument) {
	(void)argument;
	TaskSys task;
	task.init();
	task.loop();
}
