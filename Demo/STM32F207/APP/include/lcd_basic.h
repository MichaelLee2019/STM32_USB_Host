#ifndef __LCD_BASIC_H
#define __LCD_BASIC_H

void LCD_Fastlines(u16 x,u16 y,u16 len,u8 direction);		   //���ٻ��ߺ�����������ʼ���ָ꣬�����Ȼ�һ�����߻�����
void LCD_Fast_Brokenlines(u16 x, u16 y, u16 len, u8 direction);//���ٻ����ߺ�����������ʼ���ָ꣬�����Ȼ�һ�������߻�������
void LCD_Line(u16 x1,u16 y1,u16 x2,u16 y2);			 		   //ͨ�û��ߺ�������LCDָ����ʼ����ͽ�������֮�仭һ��ָ����ɫ��ֱ��
void LCD_DrawRectangle(u16 x1,u16 y1,u16 x2,u16 y2); 		   //��LCDָ����ʼ����ͽ�������֮�仭һ��ָ����ɫ�ľ���

#endif//__LCD_BASIC_H
