#include "sys.h"

#include "HW_Init.h"

/*******************************************************************************
* ������	: NVIC_Configuration
* ����  	: �ж�����
* ����  	: ��
* ����ֵ	: ��
*******************************************************************************/
void NVIC_Configuration(void)
{
	//�����ж���������Flash�ƫ������C000H(32Kb,bootloaderռ��28K)(ƫ����������100H��������)����ӳ���ж�������(�ڿ�����һ�ж�֮ǰִ��)
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);//BootLoader���ж�������0������������0x8000(32Kb)
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//ֻ��0-15����ռ���ȼ�
}

/*******************************************************************************
* ������	: HW_Init
* ����  	: Ӳ����ʼ��
* ����  	: ��
* ����ֵ	: ��
*******************************************************************************/
void HW_Init(void)
{    	
	NVIC_Configuration();    //�жϷ���
	uart5_init(115200);		 //	
	SPI1_FLASH_Init();
	LED_Init();              //LED��ʼ��
	KEY_Init();              //������ʼ��
	USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_MSC_cb, &USR_cb);//USB��ʼ��
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR , ENABLE);//ʹ��PWR��BKP����ʱ�� 
	PWR_BackupAccessCmd(ENABLE);//ʹ�ܺ󱸼Ĵ�������
}
