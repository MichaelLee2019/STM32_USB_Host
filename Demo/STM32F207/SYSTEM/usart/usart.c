#include "sys.h"
#include "stm32f2xx.h"

#include "usart.h"

//struct __FILE 
//{ 
//	int handle; 

//}; 

//FILE __stdout;

////����_sys_exit()�Ա���ʹ�ð�����ģʽ    
//_sys_exit(int x) 
//{ 
//	x = x; 
//} 

////�ض���fputc���� 
//int fputc(int ch, FILE *f)
//{      
//	USART_ClearFlag(UART5, USART_FLAG_TC);
//    USART_SendData(UART5,(uint8_t)ch);  
//	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET);  
//	return ch;
//}

void UART5_send_byte(unsigned char c)
{
	USART_ClearFlag(UART5, USART_FLAG_TC);
	USART_SendData(UART5, (u16)c);
	while(USART_GetFlagStatus(UART5, USART_FLAG_TC) != SET);//�ȴ��������
}

void USART_send_srt(unsigned char *s)
{
	while(*s != '\0')
	{
		UART5_send_byte(*s++);
	}
}

//���ַ�����ʽ����16��������
void USART_sendHEX(unsigned char hex)
{
	unsigned char const tab[]={"0123456789ABCDEF"};

	UART5_send_byte(tab[hex >> 4]);
	UART5_send_byte(tab[hex & 0x0f]);
	UART5_send_byte(' ');
}

/*******************************************************************************
* ������	: uart5_init
* ����  	: ��ʼ������1 
* ����  	: bound:������
* ����ֵ	: ��
*******************************************************************************/
void uart5_init(u32 bound)
{
    //GPIO�˿�����
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIObʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);//UART5
    
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);//��ӳ�䣬TX
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5); //��ӳ�䣬RX
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //��ת�ٶ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //����������ã�����/���/����/ģ��
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���ģʽ����©/����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����ģʽ������/����/����
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //��ת�ٶ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //����������ã�����/���/����/ģ��
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���ģʽ����©/����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����ģʽ������/����/����
	GPIO_Init(GPIOD, &GPIO_InitStructure); 

   //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�շ�ģʽ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    
	USART_Init(UART5, &USART_InitStructure);//��ʼ������ 
    
    //Usart1 NVIC ����	
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;//��ռ���ȼ�5
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;       //�����ȼ�3,�����ȼ�����Ϊ0������
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;          //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(UART5, ENABLE);//ʹ�ܴ���
}

/*******************************************************************************
* ������	: USART1_IRQHandler
* ����  	: ����1�жϷ�����,���յ�����Ϣ
* ����  	: ��
* ����ֵ	: ��
*******************************************************************************/
void UART5_IRQHandler(void)
{	
	if(USART_GetITStatus(UART5, USART_IT_RXNE) == SET)//���ָ����USART�жϷ������
	{

	}
}
