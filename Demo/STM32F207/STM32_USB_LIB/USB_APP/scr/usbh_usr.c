/**
  ******************************************************************************
  * @file    usbh_usr.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    09-November-2015
  * @brief   This file includes the usb host library user callbacks
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "sys.h"

#include "usb_conf.h"
#include "usbh_msc_core.h"

#include "ff.h"
#include "fatfs_usr.h"

#include "usbh_usr.h"

#define NULL 0

USB_OTG_CORE_HANDLE      USB_OTG_Core;
USBH_HOST                USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core_dev; 

static vu8 AppState = USH_USR_FS_INIT;
_usb_app usbx;//USB״̬������

//USB�ص�������˳��ִ��
//ʵ�ַ�ʽOTG_FS_IRQHandler��ö�ٳɹ���һֱִ��USBH_USR_MSC_Application
USBH_Usr_cb_TypeDef USR_cb =
{
  USBH_USR_Init,						//��ʼ��
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,				//��⵽��USB�豸����
  USBH_USR_ResetDevice,					//�����豸
  USBH_USR_DeviceDisconnected,			//��⵽���豸�Ƴ�
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,			//����豸�ٶȣ�������١�ȫ�١�����
  USBH_USR_Device_DescAvailable,		//�豸��Ϣ
  USBH_USR_DeviceAddressAssigned,	   	//�����ַ�ɹ�
  USBH_USR_Configuration_DescAvailable,	//��������
  USBH_USR_Manufacturer_String,		  	//������Ϣ���ַ�����
  USBH_USR_Product_String,			  	//��Ʒ��Ϣ���ַ�����
  USBH_USR_SerialNum_String,		   	//�豸����
  USBH_USR_EnumerationDone,			  	//ö�����
  USBH_USR_UserInput,					//�û�����Ӧ�ó���״̬��Ŀ
  USBH_USR_MSC_Application,				//USBӦ�ò���
  USBH_USR_DeviceNotSupported,			//�豸��֧��
  USBH_USR_UnrecoveredError 
};

/** @defgroup USBH_USR_Private_Functions
* @{
*/ 

void USBH_USR_Init(void){}//���Գ�ʼ��һЩ����

void USBH_USR_DeviceAttached(void)//��⵽��USB�豸����
{	
	usbx.status = HOST_USER_CONNECTION;//��⵽USB�豸����С����ʱ��
	LCD_UsrLog("USB�豸����\r\n");
}

//�޷��ָ��Ĵ���!!  	
void USBH_USR_UnrecoveredError (void)//
{
	LCD_UsrLog("�޷��ָ��Ĵ���!!!\r\n\r\n");
}

void USBH_USR_DeviceDisconnected (void)//��⵽���豸�Ƴ�
{
    //�豸�Ƴ����Ӵ����ʱ��
    usbx.status = 0;//����״̬
    usbx.fun = 0;//����
    
    AppState = USH_USR_FS_INIT;
    
	f_mount(0 , NULL);//ע��������
	
	LCD_UsrLog("�豸�Ѿ��Ƴ�\r\n");
}

void USBH_USR_ResetDevice(void)//��������
{
  /* callback for USB-Reset */
	LCD_UsrLog("��λ�豸...\r\n");
}

void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)//����ٶȣ���
{
  if(DeviceSpeed == HPRT0_PRTSPD_HIGH_SPEED)
  {
	  LCD_UsrLog("����\r\n");
  }  
  else if(DeviceSpeed == HPRT0_PRTSPD_FULL_SPEED)
  {
	  LCD_UsrLog("ȫ��\r\n");
  }
  else if(DeviceSpeed == HPRT0_PRTSPD_LOW_SPEED)
  {
	  LCD_UsrLog("����\r\n");
  }
  else
  {
	  LCD_UsrLog("�豸����\r\n");
  }
}

void USBH_USR_Device_DescAvailable(void *DeviceDesc)//�豸��Ϣ
{ 
  USBH_DevDesc_TypeDef *hs;
  hs = DeviceDesc;  
  
  LCD_UsrLog("VID : %04Xh\r\n" , (uint32_t)(*hs).idVendor); 
  LCD_UsrLog("PID : %04Xh\r\n" , (uint32_t)(*hs).idProduct);
}

void USBH_USR_DeviceAddressAssigned(void)//�����ַ�ɹ�
{
  LCD_UsrLog("�豸��ַ����ɹ�\r\n");
}

void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc, 	  //��������
										  USBH_InterfaceDesc_TypeDef *itfDesc, 
										  USBH_EpDesc_TypeDef *epDesc)
{
	USBH_InterfaceDesc_TypeDef *id; 
	id = itfDesc;   
	if((*id).bInterfaceClass==0x08)
	{
		LCD_UsrLog("���ƶ��洢���豸!\r\n"); 
	}else if((*id).bInterfaceClass==0x03)
	{
		LCD_UsrLog("HID �豸!\r\n"); 
	} 
}

//���̣��ַ�������Ϣ                                          
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
  LCD_UsrLog("Manufacturer : %s\r\n", (char *)ManufacturerString);
}

//��Ʒ��Ϣ���ַ�����
void USBH_USR_Product_String(void *ProductString)
{
  LCD_UsrLog("Product : %s\r\n", (char *)ProductString);
}

//�豸����
void USBH_USR_SerialNum_String(void *SerialNumString)
{
  LCD_UsrLog( "Serial Number : %s\r\n", (char *)SerialNumString); 
}

void USBH_USR_EnumerationDone(void)//ö�����
{
  LCD_UsrLog("ö�����\r\n");
}

//�޷�ʶ���USB�豸
void USBH_USR_DeviceNotSupported(void)
{	
  LCD_ErrLog("�豸��֧��\r\n");
}  

//�ȴ��û����밴��,ִ����һ������
USBH_USR_Status USBH_USR_UserInput(void)//����
{
  return USBH_USR_RESP_OK;
}  

//USB�ӿڵ�������
void USBH_USR_OverCurrentDetected (void)
{
	LCD_ErrLog("�˿ڵ�������!!!\r\n");
}


FIL *fpr;//�ļ�����ṹ��ָ��

int USBH_USR_MSC_Application(void)//Ӧ�ò���
{    
	fpr = &fsr;
	
	switch(AppState)
	{
		case USH_USR_FS_INIT://��ʼ����ע��FATFS������
		{
            usbx.status = HOST_USER_CLASS_ACTIVE;//����״̬
            
			FAT_flag = f_mount(0 , &FS);//ע���豸
			
			if(FAT_flag != 0)
			{
				USART_send_srt("f_mount err��%d");
				USART_sendHEX(FAT_flag);
				USART_send_srt("\r\n");
				return 0;
			}
			
			USART_send_srt("U����������\r\n");

			AppState = USH_USR_FS_READLIST;//��ת����һ��
		}break;

		case USH_USR_FS_READLIST://��ȡ�б���/�����ļ�
		{
            AppState = USH_USR_FS_WRITEFILE;
		}break;
		
		case USH_USR_FS_WRITEFILE://д���ļ�
		{
            AppState = USH_USR_FS_READLIST;
		}break;
		
		default: break;
	}
		
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
//USBö��״̬�������,��ֹUSBö��ʧ�ܵ��µ�����
//phost:USB_HOST�ṹ��ָ��
//����ֵ:0,û������
//       1,������,�ⲿ������������USB����.
u8 USBH_Check_EnumeDead(USBH_HOST *phost)
{
	static u16 errcnt=0;
	//���״̬,�����������,��˵��USB������.
	if(phost->gState==HOST_CTRL_XFER&&(phost->EnumState==ENUM_IDLE||phost->EnumState==ENUM_GET_FULL_DEV_DESC))
	{
		errcnt++;
		if(errcnt>2000)//������
		{ 
			errcnt=0;
			RCC->AHB2RSTR|=1<<7;	//USB OTG FS ��λ
			USB_OTG_BSP_mDelay(5);
			RCC->AHB2RSTR&=~(1<<7);	//��λ����  
			return 1;
		} 
	}else errcnt=0;
	return 0;
} 

void USBH_USR_DeInit(void)
{
  AppState = USH_USR_FS_INIT;
}
