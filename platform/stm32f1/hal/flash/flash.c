#include "flash.h"
#include "common.h"

static int flash_wait_done(void) {
	if (HAL_WaitBits(&FLASH->SR, FLASH_SR_BSY, 0U, FLASH_TIMEOUT) != HAL_OK) {
		return HAL_TIMEOUT;
	}
	if (FLASH->SR & (FLASH_SR_PGERR | FLASH_SR_WRPRTERR)) {
		FLASH->SR = (FLASH_SR_PGERR | FLASH_SR_WRPRTERR);	 // write-1-clear
		return HAL_NG;
	}
	if (FLASH->SR & FLASH_SR_EOP) {
		FLASH->SR = FLASH_SR_EOP;	 // clear EOP
	}
	return HAL_OK;
}

int FLASH_Unlock(void) {
	if ((FLASH->CR & FLASH_CR_LOCK) == 0U) {
		return HAL_OK;	  // đã mở
	}
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
	return (FLASH->CR & FLASH_CR_LOCK) ? HAL_NG : HAL_OK;
}

int FLASH_Lock(void) {
	FLASH->CR |= FLASH_CR_LOCK;
	return HAL_OK;
}

int FLASH_ErasePage(uint32_t page_addr) {
	if (page_addr & (FLASH_PAGE_SIZE - 1U)) {
		return HAL_INVALID;
	}
	int rc = flash_wait_done();
	if (rc != HAL_OK) {
		return rc;
	}
	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR = page_addr;
	FLASH->CR |= FLASH_CR_STRT;
	rc = flash_wait_done();
	FLASH->CR &= ~FLASH_CR_PER;
	return rc;
}

int FLASH_ProgramHalfWord(uint32_t addr, uint16_t data) {
	if (addr & 1U) {
		return HAL_INVALID;
	}
	int rc = flash_wait_done();
	if (rc != HAL_OK) {
		return rc;
	}
	FLASH->CR |= FLASH_CR_PG;
	*(volatile uint16_t *)addr = data;
	rc						   = flash_wait_done();
	FLASH->CR &= ~FLASH_CR_PG;
	return rc;
}

int FLASH_Program(uint32_t addr, const void *buf, uint32_t len) {
	if (buf == NULL || (addr & 1U) || (len & 1U)) {
		return HAL_INVALID;
	}
	const uint8_t *p = (const uint8_t *)buf;
	for (uint32_t i = 0U; i < len; i += 2U) {
		uint16_t hw = (uint16_t)(p[i] | ((uint16_t)p[i + 1U] << 8));
		int rc		= FLASH_ProgramHalfWord(addr + i, hw);
		if (rc != HAL_OK) {
			return rc;
		}
	}
	return HAL_OK;
}
