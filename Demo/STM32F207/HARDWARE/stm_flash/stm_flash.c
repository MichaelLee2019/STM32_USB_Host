#include "stm32f2xx.h"

#include "led.h"
#include "stm_flash.h"

//����12-512K����ÿҳ2K��
void erasure_flash(void)
{
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGAERR |FLASH_FLAG_PGPERR|FLASH_FLAG_WRPERR);

	//boot����ռ��FLASH_Sector_0��FLASH_Sector_1����С16*2=32kb
    FLASH_EraseSector(FLASH_Sector_2, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_3, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_4, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_5, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_6, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_7, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_8, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_9, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_10, VoltageRange_3);
	FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3);

	FLASH_Lock();
}

//���16λ
void Flash_program_16bit(unsigned short dat)
{
	static unsigned int Flash_prt = ApplicationAddress;
	
	FLASH_Unlock();
	
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGAERR |FLASH_FLAG_PGPERR|FLASH_FLAG_WRPERR);
	FLASH_ProgramHalfWord(Flash_prt, dat);
	Flash_prt += 2;
	
	FLASH_Lock();
}
