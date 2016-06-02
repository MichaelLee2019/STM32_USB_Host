#ifndef _LCD_H_
#define _LCD_H_

extern u16 POINT_COLOR;//������ɫ

//������ɫ
//#define WHITE         	 0xFFFF
//#define BLACK         	 0x0000	  
//#define BLUE         	 0x001F  
//#define BRED             0XF81F
//#define GRED 			 0XFFE0
//#define GBLUE			 0X07FF
//#define RED           	 0xF800
//#define MAGENTA       	 0xF81F
//#define GREEN         	 0x07E0
//#define CYAN          	 0x7FFF
//#define YELLOW        	 0xFFE0
//#define BROWN 			 0XBC40 //��ɫ
//#define BRRED 			 0XFC07 //�غ�ɫ
//#define GRAY  			 0X8430 //��ɫ

#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0xF800//0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0x001F
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ

void LCD_Init(void);

u16 LCD_ReadReg(u8 LCD_Reg);
void LCD_Clear_Color(u16 color);//����
void LCD_Clear(u16 color);//��������
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);//����亯��
void LCD_DrawPoint(u16 x,u16 y);//���㺯��
void Refurbish_LCD(void *tp);//ˢ������
u16 LCD_ReadPoint(u16 x, u16 y);//���㺯��
void BlockWrite(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);//�ֶ������
void Manual_Refurbish_LCD(void);//�ֶ�ˢ��
void Manual_LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color);

#endif//_LCD_H_
