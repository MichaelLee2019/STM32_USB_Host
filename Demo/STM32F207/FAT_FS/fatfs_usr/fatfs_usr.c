//#include "stm32f2xx.h"

#include "ff.h"
#include "fatfs_usr.h"

FATFS FS;	     //�ļ�ϵͳ����ṹ��
FIL fsr,fsw;   	 //�ļ�����ṹ
//FILINFO fileinfo;//�ļ���Ϣ
//DIR dir;  		 //Ŀ¼
FRESULT FAT_flag;//�ļ������ķ�����

#if 0
/*******************************************************************************
* ������	: getfree
* ����  	: -�õ�����ʣ������
* ����  	: -drv  :���̱��("0:"/"1:")
*			  -total:������
*			  -free :ʣ������
* ����ֵ	: 0,����������,�������
*******************************************************************************/
u8 getfree(u8 *drv,u32 *total,u32 *free)//��ȡ���д�
{
	FATFS *fs;
	u8 res;
    DWORD fre_clust=0, fre_sect=0, tot_sect=0;
	
	res = f_getfree((const TCHAR*)drv, &fre_clust, &fs);//"0:" or "" , ���ؽ��, **FATFS
	if(res == FR_OK)
	{
		tot_sect = (fs->n_fatent - 2) * fs->csize;//������=����*ÿ��������
		fre_sect = fre_clust * fs->csize;         //��������=���д�*ÿ��������

		*total=tot_sect>>1;	//��λΪKB
		*free=fre_sect>>1;	//��λΪKB 
		
		printf("%lu KB total drive space.\n"
			   "%lu KB available.\n",
			   *total / 2,*free / 2);//ÿ����512�ֽڣ�������*0.5K = ��K��
	}
	
	return res;
}
#endif
