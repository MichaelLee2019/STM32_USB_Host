#include "sys.h"
#include "HW_Init.h"

int main(void)
{
	HW_Init();
	
	while(1)
    {
        LED1 = 0;//����
        
        //���U��
        USBH_Process(&USB_OTG_Core, &USB_Host);//����USB����
        
        while(usbx.status == HOST_USER_CONNECTION)//���õ�USB�豸������δ���ӳɹ�
        {
            USBH_Process(&USB_OTG_Core, &USB_Host);//����USB����

            if(USBH_Check_EnumeDead(&USB_Host))	//���USB HOST �Ƿ�������?������,�����³�ʼ��
            { 	    
                USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_MSC_cb, &USR_cb);//USB��ʼ��
            }
        }
	}
}

