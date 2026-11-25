#include "eeproom.h"
#include "flash.h"

int EEPROM_Read(eeprom_data_t *out) {
	if (out == NULL) {
		return HAL_INVALID;
	}
	*out = *(const eeprom_data_t *)EEPROM_ADDR;
	return HAL_OK;
}

int EEPROM_Write(const eeprom_data_t *in) {
	if (in == NULL) {
		return HAL_INVALID;
	}
	int rc = FLASH_Unlock();
	if (rc == HAL_OK) {
		rc = FLASH_ErasePage(EEPROM_ADDR);
	}
	if (rc == HAL_OK) {
		rc = FLASH_Program(EEPROM_ADDR, in, sizeof(*in));
	}
	FLASH_Lock();
	return rc;
}

int EEPROM_SetUpdateRequest(void) {
	eeprom_data_t data;
	int rc = EEPROM_Read(&data);
	if (rc != HAL_OK) {
		return rc;
	}
	data.ota.update_request = true;
	return EEPROM_Write(&data);
}

int EEPROM_ClearUpdateRequest(void) {
	eeprom_data_t data;
	int rc = EEPROM_Read(&data);
	if (rc != HAL_OK) {
		return rc;
	}
	data.ota.update_request = false;
	return EEPROM_Write(&data);
}
