#include "stm32f2xx.h"
#include "delay.h"
#include "lcd.h"

/* ѡ��BANK1-BORSRAM1 ���� TFT����ַ��ΧΪ0X60000000~0X63FFFFFF
 * FSMC_A16 ��LCD��RS(�Ĵ���/����ѡ��)��
 * 16 bit => FSMC[24:0]��ӦHADDR[25:1]
 * �Ĵ�������ַ = 0X60000000
 * RAM����ַ = 0X60020000 = 0X60000000+2^16*2 = 0X60000000 + 0X20000 = 0X60020000
 * ��ѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼��㡣
 */

#define Bank1_LCD_D    ((u32)0x60020000)    //Disp Data ADDR
#define Bank1_LCD_C    ((u32)0x60000000)	//Disp Reg ADDR

#define Lcd_Light_ON   GPIO_SetBits(GPIOC, GPIO_Pin_3);//���⿪
#define Lcd_Light_OFF  GPIO_ResetBits(GPIOC, GPIO_Pin_3);//�����

u16 POINT_COLOR = 0x0000;//������ɫ

static void Delay(__IO u32 nCount);//��ʱ
static void delay_100ms(void);

/*******************************************************************************
* ������	: Delay
* ����  	: ��ʱ����
* ����  	: nCount:��ʱ�Ĳ���
* ����ֵ	: ��
*******************************************************************************/
static void Delay(__IO u32 nCount)
{	
	volatile int i;
	for(i=0;i<7200;i++)
    for(; nCount != 0; nCount--);
}

/*******************************************************************************
* ������	: WriteComm
* ����  	: д����
* ����  	: CMD:����
* ����ֵ	: ��
*******************************************************************************/
static void WriteComm(u16 CMD)
{			
	*(__IO u16 *) (Bank1_LCD_C) = CMD;
}

/*******************************************************************************
* ������	: WriteData
* ����  	: д����
* ����  	: tem_data:����
* ����ֵ	: ��
*******************************************************************************/
static void WriteData(u16 tem_data)
{			
	*(__IO u16 *) (Bank1_LCD_D) = tem_data;
}

/*******************************************************************************
* ������	: LCD_GPIO_Init
* ����  	: Һ��������ʼ��
*			  IO�ĳ�ʼ����Ϊ��FSMC�����ⲿSRAM�������в����ظ���Ϊ�˲�����
*			  ����ʼ��ѭ����Ҿͻ�����̲���IO�ظ���ʼ��
* ����  	: tem_data:����
* ����ֵ	: ��
*******************************************************************************/
 void LCD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Enable the FSMC Clock */
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
    
    /* config lcd gpio clock base on FSMC */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE , ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    
    /* config tft rst gpio */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;		
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* config tft back_light gpio base on the PT4101 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 	 
    GPIO_Init(GPIOC, &GPIO_InitStructure);  		   
    GPIO_ResetBits(GPIOC, GPIO_Pin_3);//tft back_light
    
    /* config tft data lines base on FSMC
	 * data lines,FSMC-D0~D15: PD 14 15 0 1,PE 7 8 9 10 11 12 13 14 15,PD 8 9 10
	 */	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//GPIO_Mode_AF
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | 
                                  GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                  GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure); 

  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);
    
    /* config tft control lines base on FSMC
	 * PD4-FSMC_NOE  :LCD-RD
     * PD5-FSMC_NWE  :LCD-WR
	 * PD7-FSMC_NE1  :LCD-CS
     * PD11-FSMC_A16 :LCD-RS
	 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);  
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
  	   
    /* tft control gpio init */	 
	GPIO_SetBits(GPIOB, GPIO_Pin_0);// RST = 1 
    GPIO_SetBits(GPIOD, GPIO_Pin_4);// RD = 1  
    GPIO_SetBits(GPIOD, GPIO_Pin_5);// WR = 1 
    GPIO_SetBits(GPIOD, GPIO_Pin_7);//	CS = 1 
}

/*******************************************************************************
* ������	: FSMC_Init
* ����  	: FSMC���߳�ʼ��
* ����  	: ��
* ����ֵ	: ��
*******************************************************************************/
static void FSMC_Init(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p; 
    
    p.FSMC_AddressSetupTime = 0x01;//��ַ����ʱ��
    p.FSMC_AddressHoldTime = 0x00; //��ַ����ʱ��
    p.FSMC_DataSetupTime = 0x02;   //���ݽ���ʱ��
    p.FSMC_BusTurnAroundDuration = 0x00;
    p.FSMC_CLKDivision = 0x00;
    p.FSMC_DataLatency = 0x00;

    p.FSMC_AccessMode = FSMC_AccessMode_B;//һ��ʹ��ģʽB������LCD
    
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p; 
    
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
    
    /* Enable FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE); 
}

/*******************************************************************************
* ������	: LCD_Init
* ����  	: Һ����ʼ��
* ����  	: ��
* ����ֵ	: ��
*******************************************************************************/
void LCD_Init(void)
{        
    LCD_GPIO_Init();
    FSMC_Init();
    Delay(100000);

	WriteComm(0xED);
	WriteData(0x01);
	WriteData(0xFE);
	
	WriteComm(0xEE);
	WriteData(0xDE);
	WriteData(0x21);
	
	WriteComm(0xF1);
	WriteData(0x01);
	WriteComm(0xDF);
	WriteData(0x10);
	
	//VCOMvoltage//
	WriteComm(0xC4);
	WriteData(0x8F);	  //5f
	
	WriteComm(0xC6);
	WriteData(0x00);
	WriteData(0xE2);
	WriteData(0xE2);
	WriteData(0xE2);
	WriteComm(0xBF);
	WriteData(0xAA);
	
	WriteComm(0xB0);
	WriteData(0x0D);
	WriteData(0x00);
	WriteData(0x0D);
	WriteData(0x00);
	WriteData(0x11);
	WriteData(0x00);
	WriteData(0x19);
	WriteData(0x00);
	WriteData(0x21);
	WriteData(0x00);
	WriteData(0x2D);
	WriteData(0x00);
	WriteData(0x3D);
	WriteData(0x00);
	WriteData(0x5D);
	WriteData(0x00);
	WriteData(0x5D);
	WriteData(0x00);
	
	WriteComm(0xB1);
	WriteData(0x80);
	WriteData(0x00);
	WriteData(0x8B);
	WriteData(0x00);
	WriteData(0x96);
	WriteData(0x00);
	
	WriteComm(0xB2);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x02);
	WriteData(0x00);
	WriteData(0x03);
	WriteData(0x00);
	
	WriteComm(0xB3);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	
	WriteComm(0xB4);
	WriteData(0x8B);
	WriteData(0x00);
	WriteData(0x96);
	WriteData(0x00);
	WriteData(0xA1);
	WriteData(0x00);
	
	WriteComm(0xB5);
	WriteData(0x02);
	WriteData(0x00);
	WriteData(0x03);
	WriteData(0x00);
	WriteData(0x04);
	WriteData(0x00);
	
	WriteComm(0xB6);
	WriteData(0x00);
	WriteData(0x00);
	
	WriteComm(0xB7);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x3F);
	WriteData(0x00);
	WriteData(0x5E);
	WriteData(0x00);
	WriteData(0x64);
	WriteData(0x00);
	WriteData(0x8C);
	WriteData(0x00);
	WriteData(0xAC);
	WriteData(0x00);
	WriteData(0xDC);
	WriteData(0x00);
	WriteData(0x70);
	WriteData(0x00);
	WriteData(0x90);
	WriteData(0x00);
	WriteData(0xEB);
	WriteData(0x00);
	WriteData(0xDC);
	WriteData(0x00);
	
	WriteComm(0xB8);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	
	WriteComm(0xBA);
	WriteData(0x24);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	
	WriteComm(0xC1);
	WriteData(0x20);
	WriteData(0x00);
	WriteData(0x54);
	WriteData(0x00);
	WriteData(0xFF);
	WriteData(0x00);
	
	WriteComm(0xC2);
	WriteData(0x0A);
	WriteData(0x00);
	WriteData(0x04);
	WriteData(0x00);
	
	WriteComm(0xC3);
	WriteData(0x3C);
	WriteData(0x00);
	WriteData(0x3A);
	WriteData(0x00);
	WriteData(0x39);
	WriteData(0x00);
	WriteData(0x37);
	WriteData(0x00);
	WriteData(0x3C);
	WriteData(0x00);
	WriteData(0x36);
	WriteData(0x00);
	WriteData(0x32);
	WriteData(0x00);
	WriteData(0x2F);
	WriteData(0x00);
	WriteData(0x2C);
	WriteData(0x00);
	WriteData(0x29);
	WriteData(0x00);
	WriteData(0x26);
	WriteData(0x00);
	WriteData(0x24);
	WriteData(0x00);
	WriteData(0x24);
	WriteData(0x00);
	WriteData(0x23);
	WriteData(0x00);
	WriteData(0x3C);
	WriteData(0x00);
	WriteData(0x36);
	WriteData(0x00);
	WriteData(0x32);
	WriteData(0x00);
	WriteData(0x2F);
	WriteData(0x00);
	WriteData(0x2C);
	WriteData(0x00);
	WriteData(0x29);
	WriteData(0x00);
	WriteData(0x26);
	WriteData(0x00);
	WriteData(0x24);
	WriteData(0x00);
	WriteData(0x24);
	WriteData(0x00);
	WriteData(0x23);
	WriteData(0x00);
	
	WriteComm(0xC4);
	WriteData(0x62);
	WriteData(0x00);
	WriteData(0x05);
	WriteData(0x00);
	WriteData(0x84);
	WriteData(0x00);
	WriteData(0xF0);
	WriteData(0x00);
	WriteData(0x18);
	WriteData(0x00);
	WriteData(0xA4);
	WriteData(0x00);
	WriteData(0x18);
	WriteData(0x00);
	WriteData(0x50);
	WriteData(0x00);
	WriteData(0x0C);
	WriteData(0x00);
	WriteData(0x17);
	WriteData(0x00);
	WriteData(0x95);
	WriteData(0x00);
	WriteData(0xF3);
	WriteData(0x00);
	WriteData(0xE6);
	WriteData(0x00);
	
	WriteComm(0xC5);
	WriteData(0x32);
	WriteData(0x00);
	WriteData(0x44);
	WriteData(0x00);
	WriteData(0x65);
	WriteData(0x00);
	WriteData(0x76);
	WriteData(0x00);
	WriteData(0x88);
	WriteData(0x00);
	
	WriteComm(0xC6);
	WriteData(0x20);
	WriteData(0x00);
	WriteData(0x17);
	WriteData(0x00);
	WriteData(0x01);
	WriteData(0x00);
	
	WriteComm(0xC7);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	
	WriteComm(0xC8);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	
	WriteComm(0xC9);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	
	WriteComm(0xE0);
	WriteData(0x16);
	WriteData(0x00);
	WriteData(0x1C);
	WriteData(0x00);
	WriteData(0x21);
	WriteData(0x00);
	WriteData(0x36);
	WriteData(0x00);
	WriteData(0x46);
	WriteData(0x00);
	WriteData(0x52);
	WriteData(0x00);
	WriteData(0x64);
	WriteData(0x00);
	WriteData(0x7A);
	WriteData(0x00);
	WriteData(0x8B);
	WriteData(0x00);
	WriteData(0x99);
	WriteData(0x00);
	WriteData(0xA8);
	WriteData(0x00);
	WriteData(0xB9);
	WriteData(0x00);
	WriteData(0xC4);
	WriteData(0x00);
	WriteData(0xCA);
	WriteData(0x00);
	WriteData(0xD2);
	WriteData(0x00);
	WriteData(0xD9);
	WriteData(0x00);
	WriteData(0xE0);
	WriteData(0x00);
	WriteData(0xF3);
	WriteData(0x00);
	
	WriteComm(0xE1);
	WriteData(0x16);
	WriteData(0x00);
	WriteData(0x1C);
	WriteData(0x00);
	WriteData(0x22);
	WriteData(0x00);
	WriteData(0x36);
	WriteData(0x00);
	WriteData(0x45);
	WriteData(0x00);
	WriteData(0x52);
	WriteData(0x00);
	WriteData(0x64);
	WriteData(0x00);
	WriteData(0x7A);
	WriteData(0x00);
	WriteData(0x8B);
	WriteData(0x00);
	WriteData(0x99);
	WriteData(0x00);
	WriteData(0xA8);
	WriteData(0x00);
	WriteData(0xB9);
	WriteData(0x00);
	WriteData(0xC4);
	WriteData(0x00);
	WriteData(0xCA);
	WriteData(0x00);
	WriteData(0xD2);
	WriteData(0x00);
	WriteData(0xD8);
	WriteData(0x00);
	WriteData(0xE0);
	WriteData(0x00);
	WriteData(0xF3);
	WriteData(0x00);
	
	WriteComm(0xE2);
	WriteData(0x05);
	WriteData(0x00);
	WriteData(0x0B);
	WriteData(0x00);
	WriteData(0x1B);
	WriteData(0x00);
	WriteData(0x34);
	WriteData(0x00);
	WriteData(0x44);
	WriteData(0x00);
	WriteData(0x4F);
	WriteData(0x00);
	WriteData(0x61);
	WriteData(0x00);
	WriteData(0x79);
	WriteData(0x00);
	WriteData(0x88);
	WriteData(0x00);
	WriteData(0x97);
	WriteData(0x00);
	WriteData(0xA6);
	WriteData(0x00);
	WriteData(0xB7);
	WriteData(0x00);
	WriteData(0xC2);
	WriteData(0x00);
	WriteData(0xC7);
	WriteData(0x00);
	WriteData(0xD1);
	WriteData(0x00);
	WriteData(0xD6);
	WriteData(0x00);
	WriteData(0xDD);
	WriteData(0x00);
	WriteData(0xF3);
	WriteData(0x00);
	WriteComm(0xE3);
	WriteData(0x05);
	WriteData(0x00);
	WriteData(0xA);
	WriteData(0x00);
	WriteData(0x1C);
	WriteData(0x00);
	WriteData(0x33);
	WriteData(0x00);
	WriteData(0x44);
	WriteData(0x00);
	WriteData(0x50);
	WriteData(0x00);
	WriteData(0x62);
	WriteData(0x00);
	WriteData(0x78);
	WriteData(0x00);
	WriteData(0x88);
	WriteData(0x00);
	WriteData(0x97);
	WriteData(0x00);
	WriteData(0xA6);
	WriteData(0x00);
	WriteData(0xB7);
	WriteData(0x00);
	WriteData(0xC2);
	WriteData(0x00);
	WriteData(0xC7);
	WriteData(0x00);
	WriteData(0xD1);
	WriteData(0x00);
	WriteData(0xD5);
	WriteData(0x00);
	WriteData(0xDD);
	WriteData(0x00);
	WriteData(0xF3);
	WriteData(0x00);
	
	WriteComm(0xE4);
	WriteData(0x01);
	WriteData(0x00);
	WriteData(0x01);
	WriteData(0x00);
	WriteData(0x02);
	WriteData(0x00);
	WriteData(0x2A);
	WriteData(0x00);
	WriteData(0x3C);
	WriteData(0x00);
	WriteData(0x4B);
	WriteData(0x00);
	WriteData(0x5D);
	WriteData(0x00);
	WriteData(0x74);
	WriteData(0x00);
	WriteData(0x84);
	WriteData(0x00);
	WriteData(0x93);
	WriteData(0x00);
	WriteData(0xA2);
	WriteData(0x00);
	WriteData(0xB3);
	WriteData(0x00);
	WriteData(0xBE);
	WriteData(0x00);
	WriteData(0xC4);
	WriteData(0x00);
	WriteData(0xCD);
	WriteData(0x00);
	WriteData(0xD3);
	WriteData(0x00);
	WriteData(0xDD);
	WriteData(0x00);
	WriteData(0xF3);
	WriteData(0x00);
	WriteComm(0xE5);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x02);
	WriteData(0x00);
	WriteData(0x29);
	WriteData(0x00);
	WriteData(0x3C);
	WriteData(0x00);
	WriteData(0x4B);
	WriteData(0x00);
	WriteData(0x5D);
	WriteData(0x00);
	WriteData(0x74);
	WriteData(0x00);
	WriteData(0x84);
	WriteData(0x00);
	WriteData(0x93);
	WriteData(0x00);
	WriteData(0xA2);
	WriteData(0x00);
	WriteData(0xB3);
	WriteData(0x00);
	WriteData(0xBE);
	WriteData(0x00);
	WriteData(0xC4);
	WriteData(0x00);
	WriteData(0xCD);
	WriteData(0x00);
	WriteData(0xD3);
	WriteData(0x00);
	WriteData(0xDC);
	WriteData(0x00);
	WriteData(0xF3);
	WriteData(0x00);
	
	WriteComm(0xE6);
	WriteData(0x11);
	WriteData(0x00);
	WriteData(0x34);
	WriteData(0x00);
	WriteData(0x56);
	WriteData(0x00);
	WriteData(0x76);
	WriteData(0x00);
	WriteData(0x77);
	WriteData(0x00);
	WriteData(0x66);
	WriteData(0x00);
	WriteData(0x88);
	WriteData(0x00);
	WriteData(0x99);
	WriteData(0x00);
	WriteData(0xBB);
	WriteData(0x00);
	WriteData(0x99);
	WriteData(0x00);
	WriteData(0x66);
	WriteData(0x00);
	WriteData(0x55);
	WriteData(0x00);
	WriteData(0x55);
	WriteData(0x00);
	WriteData(0x45);
	WriteData(0x00);
	WriteData(0x43);
	WriteData(0x00);
	WriteData(0x44);
	WriteData(0x00);
	
	WriteComm(0xE7);
	WriteData(0x32);
	WriteData(0x00);
	WriteData(0x55);
	WriteData(0x00);
	WriteData(0x76);
	WriteData(0x00);
	WriteData(0x66);
	WriteData(0x00);
	WriteData(0x67);
	WriteData(0x00);
	WriteData(0x67);
	WriteData(0x00);
	WriteData(0x87);
	WriteData(0x00);
	WriteData(0x99);
	WriteData(0x00);
	WriteData(0xBB);
	WriteData(0x00);
	WriteData(0x99);
	WriteData(0x00);
	WriteData(0x77);
	WriteData(0x00);
	WriteData(0x44);
	WriteData(0x00);
	WriteData(0x56);
	WriteData(0x00);
	WriteData(0x23); 
	WriteData(0x00);
	WriteData(0x33);
	WriteData(0x00);
	WriteData(0x45);
	WriteData(0x00);
	
	WriteComm(0xE8);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x99);
	WriteData(0x00);
	WriteData(0x87);
	WriteData(0x00);
	WriteData(0x88);
	WriteData(0x00);
	WriteData(0x77);
	WriteData(0x00);
	WriteData(0x66);
	WriteData(0x00);
	WriteData(0x88);
	WriteData(0x00);
	WriteData(0xAA);
	WriteData(0x00);
	WriteData(0xBB);
	WriteData(0x00);
	WriteData(0x99);
	WriteData(0x00);
	WriteData(0x66);
	WriteData(0x00);
	WriteData(0x55);
	WriteData(0x00);
	WriteData(0x55);
	WriteData(0x00);
	WriteData(0x44);
	WriteData(0x00);
	WriteData(0x44);
	WriteData(0x00);
	WriteData(0x55);
	WriteData(0x00);
	
	WriteComm(0xE9);
	WriteData(0xAA);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	
	WriteComm(0x00);
	WriteData(0xAA);
	
	WriteComm(0xCF);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	
	WriteComm(0xF0);
	WriteData(0x00);
	WriteData(0x50);
	WriteData(0x00);
	WriteData(0x00);
	WriteData(0x00);
	
	WriteComm(0xF3);
	WriteData(0x00);
	
	WriteComm(0xF9);
	WriteData(0x06);
	WriteData(0x10);
	WriteData(0x29);
	WriteData(0x00);
	
	WriteComm(0x3A);
	WriteData(0x55);	
	
	WriteComm(0x11);
	delay_100ms(); 
	WriteComm(0x29);
	WriteComm(0x35);
	WriteData(0x00);
	
	WriteComm(0x51);
	WriteData(0xFF);
	WriteComm(0x53);
	WriteData(0x2C);
	WriteComm(0x55);
	WriteData(0x82);
	WriteComm(0x2c);

	{
		u16 regval;
	
		regval|=(1<<7)|(0<<6)|(1<<5);

		WriteComm(0X36);
		WriteData(regval);

		//����X����
		WriteComm(0X2A); 
		WriteData(0);WriteData(0);

		WriteData((480-1)>>8);WriteData((480-1)&0XFF);

		WriteComm(0X2B); 
		WriteData(0);WriteData(0);
		WriteData((320-1)>>8);WriteData((320-1)&0XFF);  
	}

	LCD_Clear_Color(BLACK);//����
    
    Delay_ms(250);
	Lcd_Light_ON;//������
}

/*******************************************************************************
* ������	: LCD_DrawPoint
* ����  	: ���㺯��
* ����  	: -x:X������
*			  -y:y������
* ����ֵ	: ��
*******************************************************************************/
void LCD_DrawPoint(u16 x,u16 y)
{
    //���ù��λ��
	WriteComm(0x2A);
	WriteData(x>>8);
	WriteData(x&0xFF);
	WriteComm(0x2B);
	WriteData(y>>8);
	WriteData(y&0xFF);

    //��ʼд��GRAM	 
    WriteComm(0X2C);

	*(__IO u16 *) (Bank1_LCD_D) = POINT_COLOR;
}

/*******************************************************************************
* ������	: LCD_Clear_Color
* ����  	: ��������
* ����  	: ����
* ����ֵ	: ��
*******************************************************************************/
void LCD_Clear_Color(u16 color)//����
{
    u32 i;
    u32 s = 320*480;//���
    
    WriteComm(0x2A);
    WriteData(0); 
    WriteData(0);	 
    WriteComm(0x2B); 
    WriteData(0); 
    WriteData(0);
    
    //��ʼд��GRAM	 
    WriteComm(0X2C);
            
    for (i=0; i<s; i++)
    {
        *(__IO u16 *) (Bank1_LCD_D) = color;
    }
}

/*******************************************************************************
* ������	: ����亯��
* ����  	: ��ָ����������䵥����ɫ,�����СΪ:(x1-x2+1)*(y2-y2+1)
* ����  	: -  sx :x������
*			  -  sy :y������
*			  -  ex :�Խ�x������
*			  -  ey :�Խ�y������
*			  -color:Ҫ������ɫ
* ����ֵ	: ��
*******************************************************************************/
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color)
{
	u32 i;
    u32 s = 0;//���

    s = (ex-sx+1)*(ey-sy+1);
    
    //���ù��λ��
    WriteComm(0x2A); 
    WriteData(sx>>8); 
    WriteData(sx&0XFF);
    WriteData(ex>>8);
    WriteData(ex&0XFF);
    
    WriteComm(0x2B); 
    WriteData(sy>>8); 
    WriteData(sy&0XFF);
    WriteData(ey>>8); 
    WriteData(ey&0XFF);
    
    //��ʼд��GRAM	 
    WriteComm(0X2C);
            
    for (i=0; i<s; i++)
    {
        *(__IO u16 *) (Bank1_LCD_D) = color;
    }
}

static void delay_100ms(void)
{
	u32 i=800000;
	
	while(i--);	
}
