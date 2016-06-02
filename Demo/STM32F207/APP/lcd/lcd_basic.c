#include "stm32f2xx.h"

#include "lcd.h"
#include "lcd_basic.h"

/*******************************************************************************
* ������	: LCD_Fastlines
* ����  	: ���ٻ��ߺ���
* ����  	: -    x    :x������
*			  -    y    :y������
*			  -   len   :����
*			  -direction:����0�������ߣ�1�������ߣ�
* ����ֵ	: ��
*******************************************************************************/
void LCD_Fastlines(u16 x,u16 y,u16 len,u8 direction)
{
	u16 i;
	
	if(direction == 0)
	{
		if((x+len) > 480) return;	//������Ļ��Χ���˳�
		
		for(i=0;i<len;i++)
		{
		 	LCD_DrawPoint(x+i, y);
		}
	}
	else if(direction==1)
	{
		if((y+len) > 320) return;	//������Ļ��Χ���˳�
		
		for(i=0;i<len;i++)
		{
		 	LCD_DrawPoint(x, y+i);
		}
	}
}

/*******************************************************************************
* ������	: LCD_Fast_Brokenlines
* ����  	: ���ٻ����ߺ���
* ����  	: -    x    :x������
*			  -    y    :y������
*			  -   len   :����
*			  -interval :���
*			  -direction:����0�������ߣ�1�������ߣ�
* ����ֵ	: ��
*******************************************************************************/
void LCD_Horizontal_Dottedlines(u16 x, u16 y, u16 len, u8 interval, u8 direction)//ˮƽ������
{
	s16 i;
		
	if(direction == 0)//��������
	{
		if((x+len) > 480) return;	//������Ļ��Χ���˳�
		
		for(i=0;i<len;i+=interval)
		{
		 	LCD_DrawPoint(x+i, y);
		}
	}
	else if(direction == 1)//��������
	{
		if((x-(len-1)) < 0) return;	//������Ļ��Χ���˳�
		
		for(i=x;i>x-(len-1);i-=interval)
		{
		 	LCD_DrawPoint(i, y);
		}
	}
}

void LCD_Vertical_Dottedlines(u16 x, u16 y, u16 len, u8 interval, u8 direction)//��ֱ������
{
	s16 i;
		
	if(direction == 0)//��������
	{
		if((y+len) > 320) return;//������Ļ��Χ���˳�
		
		for(i=0;i<len;i+=interval)
		{
		 	LCD_DrawPoint(x, y+i);
		}
	}
	else if(direction == 1)//��������
	{
		if((y-len) < 0) return;//������Ļ��Χ���˳�
		
		for(i=y;i>y-(len-1);i-=interval)
		{
		 	LCD_DrawPoint(x, i);
		}
	}
}

/*******************************************************************************
* ������	: LCD_Line
* ����  	: ͨ�û��ߺ��������㻭��
* ����  	: -x1:x1������
*			  -y1:y1������
*			  -x2:x2������
*			  -y2:y2������
* ����ֵ	: ��
*******************************************************************************/
void LCD_Line(u16 x1,u16 y1,u16 x2,u16 y2)
{
	unsigned int x,y; 
	unsigned int d_x,d_y;//d_x=x2-x1;d_y=y2-y1;
	int err=0;
	unsigned char temp=0;
	
	POINT_COLOR=WHITE;
	if(((x1>480)&&(x2>480))&&((y1>320)&&(y2>320))) return;	//������Ļ��Χ���˳�
	if(y2<y1)
	{
		x=x1;
		y=y1;
		x1=x2;
		y1=y2;
		x2=x;
		y2=y;
	}
	d_y=y2-y1; 			//�������Ծ���
	if (d_y==0)			//�������X���겻��ȣ�Y�������ʱ����һ��ˮƽ��
	{
		if (x1>x2) 
		{
			x=x1;
			x1=x2;
			x2=x;
		}
		for (x=x1;x<=x2;x++) 
			LCD_DrawPoint(x,y1);
	}

	else			   //
	{
		if(x2>=x1)
		{
			temp=1;
			d_x=x2-x1;
		}
		else 
			d_x=x1-x2;
		x=x1;
		y=y1;
		LCD_DrawPoint(x,y);

		if(temp&&(d_y<=d_x))  //d_y>0,d_x>0,x2>=x1
			while(x!=x2)	  //x1!=x2
			{
				if(err<0)
				{
					x=x+1;
					err=err+(y2-y);
				}
				else 
				{
					x=x+1;
					y=y+1;
					err=err+(y2-y)-(x2-x);//err=err+d_y-d_x
				}
				LCD_DrawPoint(x,y);
			}

		else if(temp&&(d_y>d_x))
			while(y!=y2)
			{
				d_x=x2-x;
				d_y=y2-y; 
				if(err<0)
				{
					x=x+1;
					y=y+1;
					err=err+d_y-d_x;
				}
				else 
				{
					y=y+1;
					err=err-d_x;
				}
				LCD_DrawPoint(x,y);

			}

		else if(!temp&&(d_y<=d_x))
			while(x!=x2)
			{
				d_x=x-x2;
				d_y=y2-y; 
				if(err<0)
				{
					x=x-1;
					err=err+d_y;
				}
				else 
				{
					x=x-1;
					y=y+1;
					err=err+d_y-d_x;
				}
				LCD_DrawPoint(x,y);
			}

		else if(!temp &&(d_y>d_x))
			while(y!=y2)
			{
				d_x=x-x2;
				d_y=y2-y; 
				if(err<0)
				{
					x=x-1;
					y=y+1;
					err=err+d_y-d_x;
				}
				else 
				{
					y=y+1;
					err=err-d_x;
				}
				LCD_DrawPoint(x,y);
			}
	}
}

/*******************************************************************************
* ������	: LCD_DrawRectangle
* ����  	: �����κ���
* ����  	: -x1:x1������
*			  -y1:y1������
*			  -x2:x2������
*			  -y2:y2������
* ����ֵ	: ��
*******************************************************************************/
void LCD_DrawRectangle(u16 x1,u16 y1,u16 x2,u16 y2)
{
	unsigned char Temp;
        
	if(x1>x2)
	{
		Temp=x2;
		x2=x1;
		x2=Temp;
	}	
	if(y1>y2)
	{
		Temp=y2;
		y2=y1;
		y1=Temp;
	}
	LCD_Fastlines(x1,y1,x2-x1+1,0);//����
	LCD_Fastlines(x1,y2,x2-x1+1,0);//����

	LCD_Fastlines(x1,y1,y2-y1+1,1);//����
	LCD_Fastlines(x2,y1,y2-y1+1,1);//����
}


