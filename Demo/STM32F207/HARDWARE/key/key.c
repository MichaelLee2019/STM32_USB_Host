#include "stm32f2xx.h"

#include "delay.h"
#include "key.h"

//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
    
	//��ʼ��KEY0-->GPIOA.13,KEY1-->GPIOA.15  ��������
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��PORTA,PORTEʱ��

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����������ã�����/���/����/ģ��
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����ģʽ������
	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_13 | GPIO_Pin_15;//PG12��PG13
 	GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ��GPIOG13��15
}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��S1����
//2��S2����
//3��S3���� 
//4��S4����
//5��S5����
//6��S6���� 
//ע��˺�������Ӧ���ȼ�,S1>S2>S3>S4>S5>S6!!
/*u8 KEY_Scan(u8 mode)
{	
	static u8 key_up = 1;//�������ɿ���־
	static u8 key_cnt = 0;
	static u8 key_value;

	Delay_ms(5);
	
	if(key_up == 1)
	{
		key_value = 0;
	}
	
	if(mode && (key_cnt++ >= 50))//֧������
	{
		key_up = 1;
		key_cnt = 50;
	}
	
	if((KEY6 == 0) && (KEY1 == 0))//��ϰ���
	{
		key_value = 7;
		goto END;//ֱ�ӷ��ذ���ֵ
	}	

	if((KEY6 == 0) && (KEY2 == 0))//��ϰ���
	{
		key_value = 8;
		goto END;//ֱ�ӷ��ذ���ֵ
	}
	
	if(key_up && (KEY1==0 || KEY2==0 || KEY3==0 || KEY4==0 || KEY5==0 || KEY6==0))
	{
		Delay_ms(5);//ȥ���� 
		
		if(mode == 1)
		{
			key_up = 0;
			
			if     (KEY1 == 0) return 1;
			else if(KEY2 == 0) return 2;
			else if(KEY3 == 0) return 3;
			else if(KEY4 == 0) return 4;
			else if(KEY5 == 0) return 5;
			else if(KEY6 == 0) return 6;
		}
		else
		{
			key_up = 0;

			if     (KEY1 == 0) key_value = 1;
			else if(KEY2 == 0) key_value = 2;
			else if(KEY3 == 0) key_value = 3;
			else if(KEY4 == 0) key_value = 4;
			else if(KEY5 == 0) key_value = 5;
			else if(KEY6 == 0) key_value = 6;
		}			
	}
	else if(KEY1==1 && KEY2==1 && KEY3==1 && KEY4==1 && KEY5==1 && KEY6==1)
	{
		END:
		
		key_up = 1;
		key_cnt = 0;
		
		return key_value;
	}

	return 0;// �ް�������
}*/
