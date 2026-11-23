extern "C" {
#include "baros.h"
}

#include "timer.h"

void Timer::set_period(uint32_t ms) {
	period_ = ms;
	last_	= baros_millis();
}

bool Timer::is_due() {
	if ((uint32_t)(baros_millis() - last_) >= period_) {
		last_ += period_;
		return true;
	}
	return false;
}