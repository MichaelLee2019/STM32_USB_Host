#ifndef __USART_H
#define __USART_H

extern unsigned char need_IAP_r;//�������ִ��ڽ��յ���bin���ݻ���IAP����
extern volatile unsigned char rcv_frame_r;//������ɱ�־
extern unsigned char receive[256];//���ջ�����
extern unsigned char receive_r[256];//���ջ�����

void USART_send_srt(unsigned char *s);
void USART_sendHEX(unsigned char hex);

void uart5_init(u32 bound);//��ʼ������1

void USART_Upgrade_System(void);//��������ϵͳ
void USART_Upgrade_Fort(void);//�����ֿ�����

#endif


