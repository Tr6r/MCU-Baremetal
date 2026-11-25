#ifndef __FLASH__
#define __FLASH__

#include "stm32f1.h"

#define FLASH_REG_BASE (PERIPHERAL_ADDRESS + 0x00022000U)

typedef struct {
	volatile uint32_t ACR;		   // 0x00 Access control: latency, prefetch, half-cycle access
	volatile uint32_t KEYR;		   // 0x04 FPEC key: write KEY1 then KEY2 to unlock CR
	volatile uint32_t OPTKEYR;	   // 0x08 Option-byte key: unlock option-byte programming
	volatile uint32_t SR;		   // 0x0C Status: BSY, PGERR, WRPRTERR, EOP
	volatile uint32_t CR;		   // 0x10 Control: PG, PER, MER, OPTPG, OPTER, STRT, LOCK, interrupts
	volatile uint32_t AR;		   // 0x14 Address: page address for erase / program target
	volatile uint32_t RESERVED;	   // 0x18 Reserved
	volatile uint32_t OBR;		   // 0x1C Option byte: read-protect, user option bytes
	volatile uint32_t WRPR;		   // 0x20 Write protection: per-page write-protect bits
} FLASH_TypeDef;

#define FLASH ((FLASH_TypeDef *)FLASH_REG_BASE)

#define FLASH_KEY1 0x45670123U
#define FLASH_KEY2 0xCDEF89ABU

// SR
#define FLASH_SR_BSY	  (1U << 0)
#define FLASH_SR_PGERR	  (1U << 2)
#define FLASH_SR_WRPRTERR (1U << 4)
#define FLASH_SR_EOP	  (1U << 5)

// CR
#define FLASH_CR_PG	  (1U << 0)	   // program
#define FLASH_CR_PER  (1U << 1)	   // page erase
#define FLASH_CR_MER  (1U << 2)	   // mass erase
#define FLASH_CR_STRT (1U << 6)	   // start
#define FLASH_CR_LOCK (1U << 7)	   // lock

#define FLASH_PAGE_SIZE 1024U   

#define FLASH_TIMEOUT 1000000U


int FLASH_Unlock(void);
int FLASH_Lock(void);
int FLASH_ErasePage(uint32_t page_addr);               /* addr căn 1K */
int FLASH_ProgramHalfWord(uint32_t addr, uint16_t data);
int FLASH_Program(uint32_t addr, const void *buf, uint32_t len);

#endif	  //__FLASH__
