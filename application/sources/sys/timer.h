#ifndef __TIMER__
#define __TIMER__

#include <stdint.h>

class Timer {
public:
	void set_period(uint32_t ms);
	bool is_due();

private:
	uint32_t last_;
	uint32_t period_;
};

#endif	  //__TIMER__
