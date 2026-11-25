#ifndef __EEPROM__
#define __EEPROM__

#include "stm32f1.h"

#define EEPROM_ADDR	 0x08003000U

typedef struct {
	uint32_t version;
	bool	 update_request;
} ota_info_t;

typedef struct {
	ota_info_t ota;
} eeprom_data_t;

int EEPROM_Read(eeprom_data_t *out);
int EEPROM_Write(const eeprom_data_t *in);
int EEPROM_SetUpdateRequest(void);
int EEPROM_ClearUpdateRequest(void);

#endif	  //__EEPROM__
