#include <string.h>

#include "stm32f2xx.h"

#include "main.h"
#include "sys.h"
#include "usart.h"

#include "menu.h"

char over=0; //������־

//==================================== 
//�ṹ������ 
struct MenuItem m0_main[2];
struct MenuItem m1_USART[2]; 
struct MenuItem m1_Flash_Disk[2];

//�������� 
void Nop(void);
void Run(struct MenuItem *manyou);
void USART_IAP(void);//���ڲ�������
void IAP_FROM_Flash_Disk(void);//U������ϵͳ
void IAP_FROM_Font_Disk(void);//U�������ֿ�

//==================================== 
//�ṹ��ʵ�� 
struct MenuItem m0_main[2]= 
{ 
	{1,120,123,"1 USART Drive       ",Nop,m1_USART,Null}, 
	{1,120,171,"2 Generic Flash Disk",Nop,m1_Flash_Disk,Null},
};

struct MenuItem m1_USART[2]= 
{ 
	{2,120,123,"1 System Upgrade",USART_IAP,Null,m0_main}, 
	{2,120,171,"2 Fort Upgrade  ",USART_IAP,Null,m0_main}, 
};

struct MenuItem m1_Flash_Disk[2]= 
{ 
	{2,120,123,"1 System Upgrade",IAP_FROM_Flash_Disk,Null,m0_main}, 
	{2,120,171,"2 Fort Upgrade  ",IAP_FROM_Font_Disk,Null,m0_main},
};

//==================================== 
void Run(struct MenuItem *currentMenu) 
{ 
	(*(currentMenu->Subs))(currentMenu);
}

void Nop(void) 
{}

void USART_IAP(void)//���ڲ�������
{
	while(1)//�ȴ���λ����������
	{
		if(rcv_frame_r == 1)
		{
			if(strcmp((char*)receive_r, "$07*") == 0)//����ע����������
			{
				RTC_WriteBackupRegister(UPDATA_RTC_BKP, IAP_FROM_USART);//д��IAPԼ����ʶ
				SystemReset();//��λϵͳ
			}
			
			if(strcmp((char*)receive_r, "$TS*") == 0)//�����س����ֿ⵽FLASH��
			{	
				RTC_WriteBackupRegister(UPDATA_RTC_BKP, IAP_Font_USART);//д��IAPԼ����ʶ
				SystemReset();//��λϵͳ
			}			
		}
	}
}

void IAP_FROM_Flash_Disk(void)
{
	RTC_WriteBackupRegister(UPDATA_RTC_BKP, IAP_FROM_USB_Flash_Disk);//д��IAPԼ����ʶ
	SystemReset();//��λϵͳ
}

void IAP_FROM_Font_Disk(void)
{
	RTC_WriteBackupRegister(UPDATA_RTC_BKP, IAP_Font_USB_Flash_Disk);//д��IAPԼ����ʶ
	SystemReset();//��λϵͳ	
}
