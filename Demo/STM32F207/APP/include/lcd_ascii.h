#ifndef __LCD_ASCII_H
#define __LCD_ASCII_H
																						 
void LCD_printc(u16 x, u16 y, u8 c_dat,u8 size,u8 b);	//��ʾ�ַ���sizeΪ12��16��24��32��40��48�ֱ���6*12��8*16��12*24��16*32��20*40��24*48
void LCD_prints(u16 x, u16 y, u8 *s_dat,u8 size,u8 b);//��ʾ�ַ�����//sizeΪ12��16��24��32��40��48�ֱ���6*12��8*16��12*24��16*32��20*40��24*48

#endif




