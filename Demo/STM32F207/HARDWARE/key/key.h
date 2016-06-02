#ifndef __KEY_H
#define __KEY_H	 

//USART1 mode �ߵ�ƽ��Ч
//#define U1_MOD		   (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_15))//����WK_UP��ϵͳ����
//#define U2_MOD		   (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_13))//����WK_UP��ϵͳ����

#define KEY6  (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_15))//��ȡ����6��exit��
#define KEY5  (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)) //��ȡ����5��ok��
#define KEY4  (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_14))//��ȡ����4������ 
#define KEY3  (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_12))//��ȡ����3������
#define KEY2  (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_11))//��ȡ����2������ 
#define KEY1  (GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_13))//��ȡ����1������ 

#define UP 	  3
#define Down  4
#define Left  1
#define Right 2
#define Enter 5
#define Esc   6
#define Reset 7

void KEY_Init(void); //������ʼ��
//u8 KEY_Scan(u8 mode);//����ɨ�躯��

#endif
