#include "stm32f2xx.h"

#include "integer.h"
#include "fattime.h"

DWORD get_fattime (void)
{
//	u32 ttime;
	u32 date=0;
//	RTC_HEX t;
//    
//	xQueueReceive(xQueueHandle_Sys_time, &t, 1000);//��ȡ�������ȴ�1s
//	
//    ttime = HEX2BCD(t.year)-1984;	 //�õ����
// 	date |= (ttime<<25);
//    ttime = HEX2BCD(t.mon);	 //�õ��·�
// 	date |= (ttime<<21);
//	ttime = HEX2BCD(t.day);	  	 //�õ�����
// 	date |= (ttime<<16);
//	ttime = HEX2BCD(t.hour);  	 //�õ�ʱ��
// 	date |= (ttime<<11);
//	ttime = HEX2BCD(t.min);	 //�õ�����
// 	date |= (ttime<<5);
// 	date |= (HEX2BCD(t.sec)/2);//�õ�����	
	
    return date;
}
