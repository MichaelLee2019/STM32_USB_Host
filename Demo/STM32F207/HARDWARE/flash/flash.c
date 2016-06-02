#include "stm32f2xx.h"

#include "flash.h" 

#define  Dummy_byte 0xAA

#define	CS_0    GPIO_ResetBits(GPIOA, GPIO_Pin_4);
#define	CS_1    GPIO_SetBits(GPIOA, GPIO_Pin_4);

u16 W25QXX_TYPE = W25Q64;	//Ĭ����W25Q64

//��������
void SPI_Flash_Wait_Busy(void);

/*******************************************************************************
* ������	: SPI1_FLASH_Init
* ����  	: SPI1�ӿڳ�ʼ��
* ����  	: ��
* ����ֵ	: ��
*******************************************************************************/
void SPI1_FLASH_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//ʹ��APB2����ʱ��
			
	/* Enable SPI1 and GPIO clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 , ENABLE);
	
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
    
	/* Configure SPI1 pins: CS */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���츴��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
    
	/* Configure SPI1 pins: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���츴��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Deselect the FLASH: Chip Select high */
	GPIO_SetBits(GPIOA, GPIO_Pin_4);//CS=1��
	
	/* SPI1 configuration */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;					  //��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				  //8bit
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						  //CS���������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;//Ԥ��Ƶ2:36MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				  //��λΪ���λ
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE);
}

/*******************************************************************************
* ������	: SPI1_ReadWriteByte
* ����  	: SPI1��д����
* ����  	: dat:Ҫд�������
* ����ֵ	: ��ȡ�����ֽ�
*******************************************************************************/
u8 SPI1_ReadWriteByte(u8 dat)
{		
	u8 r;	
	
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	
	SPI_I2S_SendData(SPI1, dat); //ͨ������SPIx����һ������

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
		
	r = SPI_I2S_ReceiveData(SPI1);
	
	return(r);			    
}

/*******************************************************************************
* ������	: SPI_Flash_ReadSR
* ����  	: ��ȡSPI_FLASH��״̬�Ĵ���
* ����  	: ��
* ����ֵ	: //BIT7  6   5   4   3   2   1   0
*				SPR   RV  TB BP2 BP1 BP0 WEL BUSY
*				SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
*				TB,BP2,BP1,BP0:FLASH����д��������
*				WEL:дʹ������
*				BUSY:æ���λ(1,æ;0,����)
*				Ĭ��:0x00
*******************************************************************************/
u8 SPI_Flash_ReadSR(void)   
{  
	u8 r = 0;   
	
	CS_0
	
	SPI1_ReadWriteByte(W25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	r = SPI1_ReadWriteByte(Dummy_byte);        //��ȡһ���ֽ�  
	
	CS_1
	
	return(r);   
} 

/*******************************************************************************
* ������	: SPI_FLASH_Write_Enable
* ����  	: SPI_FLASHдʹ��
* ����  	: ��
* ����ֵ	: ��
*******************************************************************************/
void SPI_FLASH_Write_Enable(void)   
{
	CS_0
	
    SPI1_ReadWriteByte(W25X_WriteEnable);      //����дʹ��  
	
	CS_1
} 

/*******************************************************************************
* ������	: SPI_Flash_ReadID
* ����  	: ��ȡоƬID
* ����  	: ��
* ����ֵ	: оƬID
*******************************************************************************/
//u16 SPI_Flash_ReadID(void)
//{
//	u16 ID = 0;	 
//	 
//	CS_0	
//		    
//	SPI1_ReadWriteByte(0x90);//���Ͷ�ȡID����	    
//	SPI1_ReadWriteByte(0x00); 	    
//	SPI1_ReadWriteByte(0x00); 	    
//	SPI1_ReadWriteByte(0x00); 	 			   
//	ID |= SPI1_ReadWriteByte(Dummy_byte)<<8;  
//	ID |= SPI1_ReadWriteByte(Dummy_byte);	 

//	CS_1
//			    
//	return(ID);
//}   	

/*******************************************************************************
* ������	: SPI_Flash_Read
* ����  	: ������(���65535)
* ����  	: -buf:���ݻ�����ָ��
*			  -add:���ݵ�ַ
*			  -len:��ȡ����
* ����ֵ	: ��
*******************************************************************************/
//void SPI_Flash_Read(u8* buf, u32 add,u16 len)   
//{ 
// 	u16 i;   
//	 												    
//	CS_0

//    SPI1_ReadWriteByte(W25X_ReadData);    //���Ͷ�ȡ����   
//	
//    SPI1_ReadWriteByte((u8)((add)>>16));  //����24bit��ַ    
//    SPI1_ReadWriteByte((u8)((add)>>8));   
//    SPI1_ReadWriteByte((u8)add);   
//	
//    for(i=0;i<len;i++)
//	{ 
//        buf[i] = SPI1_ReadWriteByte(Dummy_byte);   //ѭ������  
//    }

//	CS_1	      
//}  

/*******************************************************************************
* ������	: SPI_Flash_Write_Page
* ����  	: SPI��һҳ(0~65535)��д������256���ֽڵ�����
* ����  	: -buf:���ݻ�����ָ��
*			  -add:���ݵ�ַ
*			  -len:д�볤��
* ����ֵ	: ��
*******************************************************************************/
void SPI_Flash_Write_Page(u8* buf, u32 add, u16 len)
{
 	u16 i;  
	
    SPI_FLASH_Write_Enable();         
	
	CS_0
	   
    SPI1_ReadWriteByte(W25X_PageProgram); //����дҳ����   
	
    SPI1_ReadWriteByte((u8)((add)>>16)); //����24bit��ַ    
    SPI1_ReadWriteByte((u8)((add)>>8));   
    SPI1_ReadWriteByte((u8)add);   
	
    for(i=0;i<len;i++)
	{
		SPI1_ReadWriteByte(buf[i]);//ѭ��д��  
	}
	
	CS_1
	
	SPI_Flash_Wait_Busy();					   //�ȴ�д�����
} 

/*******************************************************************************
* ������	: SPI_Flash_Write
* ����  	: �޼���дSPI FLASH 
* ����  	: -buf:���ݻ�����ָ��
*			  -add:���ݵ�ַ
*			  -len:д�볤��
* ����ֵ	: ��
*******************************************************************************/
void SPI_Flash_Write(u8* buf, u32 add, u16 len)   
{ 			 		 
	u16 start;
		   
	start = 256 - (add & 255); //��ҳʣ����ֽ���
			 	    
	if(len <= start)
	{
		start = len;//������256���ֽ�
	}
	
	while(1)
	{	   
		SPI_Flash_Write_Page(buf, add, start);
		
		if(len == start)
		{
			break;
		}
	 	else
		{
			buf += start;
			add += start;	

			len -= start;		
			
			if(len > 256)
			{
				start = 256;
			}
			else
			{
				start = len; 	  //����256���ֽ���
			}
		}
	}    
} 

/*******************************************************************************
* ������	: SPI_Flash_Erase_Chip
* ����  	: ��������оƬ,�ȴ�ʱ�䳬��...
* ����  	: ��
* ����ֵ	: ��
*******************************************************************************/
//void SPI_Flash_Erase_Chip(void)   
//{                                             
//    SPI_FLASH_Write_Enable();                  //SET WEL 
//    SPI_Flash_Wait_Busy();   

//	CS_0

//    SPI1_ReadWriteByte(W25X_ChipErase);        //����Ƭ��������  

//	CS_1
//      
//	SPI_Flash_Wait_Busy();   				   //�ȴ�оƬ��������
//}

/*******************************************************************************
* ������	: SPI_Flash_Erase_Sector
* ����  	: ����һ������:150ms
* ����  	: Dst_Addr:������ַ
* ����ֵ	: ��
*******************************************************************************/
void SPI_Flash_Erase_Sector(u32 Dst_Addr)   
{   
    SPI_FLASH_Write_Enable();                  //SET WEL 	 
    SPI_Flash_Wait_Busy();   

	CS_0

    SPI1_ReadWriteByte(W25X_SectorErase);      //������������ָ�� 
	
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>16));  //����24bit��ַ    
    SPI1_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPI1_ReadWriteByte((u8)Dst_Addr);  

	CS_1

    SPI_Flash_Wait_Busy();   				   //�ȴ��������
} 
 
/*******************************************************************************
* ������	: SPI_Flash_Wait_Busy
* ����  	: �ȴ�����
* ����  	: ��
* ����ֵ	: ��
*******************************************************************************/
void SPI_Flash_Wait_Busy(void)   
{   
	while((SPI_Flash_ReadSR()&0x01) == 0x01);   // �ȴ�BUSYλ���
}  

/*******************************************************************************
* ������	: SPI_Flash_Write_Data
* ����  	: �Ѳɼ�����д��FLASH��ֻ����ǰд
* ����  	: -buf:���ݴ洢��
*			  -add:��ʼд��ĵ�ַ(24bit)
*			  -len:Ҫд����ֽ���
* ����ֵ	: ��
*******************************************************************************/
//void SPI_Flash_Write_Data(u8 *buf, u32 add, u16 len)
//{
//    u16 Residue;	//ʣ���С
//	u32 Erasure_Add;//Ҫ������������ַ����Χ��
//	
//	Residue = 4096-(add&4095);//��Ҫд�ĵ�ַȡ��4kb��һ�����������������������ʣ���С��4095�Ķ����ƣ�1111 1111 1111��add&4095 == add%4096
//		
//	if(Residue < len)//�������ʣ���СС��Ҫд������ݳ��ȣ���������ֿ�����д���ݣ��������Ȳ�����һ������
//	{		
//		Erasure_Add = (add&0xFFFFF000) + 4096;//��������һ������
//		SPI_Flash_Erase_Sector(Erasure_Add);
//		
////		printf(">>>>>>>>��������:%X\r\n",Erasure_Add);
//	}
//	else if(Residue == 4096)//�����һ����������ʼ��ַ��
//	{		
//		SPI_Flash_Erase_Sector(add);//������ǰ����
//		
////		printf("********��������:%X\r\n",add);
//	}
//	
//	SPI_Flash_Write(buf, add, len);
//}
