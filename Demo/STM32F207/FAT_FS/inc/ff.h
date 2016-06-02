/*---------------------------------------------------------------------------/
/  FatFs - FAT file system module include file  R0.09b    (C)ChaN, 2013
/----------------------------------------------------------------------------/
/ FatFs module is a generic FAT file system module for small embedded systems.
/ This is a free software that opened for education, research and commercial
/ developments under license policy of following terms.
/
/  Copyright (C) 2013, ChaN, all right reserved.
/
/ * The FatFs module is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/----------------------------------------------------------------------------*/

#ifndef _FATFS
#define _FATFS	82786	/* Revision ID */

#ifdef __cplusplus
extern "C" {
#endif

#include "integer.h"	/* Basic integer types */
#include "ffconf.h"		/* FatFs configuration options */

#if _FATFS != _FFCONF
#error Wrong configuration file (ffconf.h).
#endif



/* Definitions of volume management */

#if _MULTI_PARTITION		/* Multiple partition configuration */
typedef struct {
	BYTE pd;	/* Physical drive number */
	BYTE pt;	/* Partition: 0:Auto detect, 1-4:Forced partition) */
} PARTITION;
extern PARTITION VolToPart[];	/* Volume - Partition resolution table */
#define LD2PD(vol) (VolToPart[vol].pd)	/* Get physical drive number */
#define LD2PT(vol) (VolToPart[vol].pt)	/* Get partition index */

#else							/* Single partition configuration */
#define LD2PD(vol) (BYTE)(vol)	/* Each logical drive is bound to the same physical drive number */
#define LD2PT(vol) 0			/* Always mounts the 1st partition or in SFD */

#endif



/* Type of path name strings on FatFs API */

#if _LFN_UNICODE			/* Unicode string */
#if !_USE_LFN
#error _LFN_UNICODE must be 0 in non-LFN cfg.
#endif
#ifndef _INC_TCHAR
typedef WCHAR TCHAR;
#define _T(x) L ## x
#define _TEXT(x) L ## x
#endif

#else						/* ANSI/OEM string */
#ifndef _INC_TCHAR
typedef char TCHAR;
#define _T(x) x
#define _TEXT(x) x
#endif

#endif



/* File system object structure (FATFS) ���ļ�ϵͳ����ṹ�塿*/

typedef struct {
	BYTE	fs_type;		/* FAT sub-type (0:Not mounted) */									//��FAT�����ͣ�һ����mountʱ�ã���0��ʾδ���ء�
	BYTE	drv;			/* Physical drive number*/ 											//�����������ţ�һ��Ϊ0��
	BYTE	csize;			/* Sectors per cluster (1,2,4...128) */								//��ÿ���ص�������Ŀ(1,2,4...128)��
	BYTE	n_fats;			/* Number of FAT copies (1,2) */									//���ļ���������Ŀ(1,2)��
	BYTE	wflag;			/* win[] dirty flag (1:must be written back) */						//������ļ��Ƿ񱻸Ķ�����Ϊ1ʱҪ��д��
	/*FAT�ļ�ϵͳ����Ϊ�����������������ļ��������Ŀ¼����������*/
	BYTE	fsi_flag;		/* fsinfo dirty flag (1:must be written back) */					//������ļ�ϵͳ��Ϣ�Ƿ񱻸Ķ�����Ϊ1ʱҪ��д��
	WORD	id;				/* File system mount ID */											//���ļ�ϵͳ����ID��
	WORD	n_rootdir;		/* Number of root directory entries (FAT12/16) */					//����Ŀ¼�����(Ŀ¼��)�ĸ���(����FAT12/16)��
#if _MAX_SS != 512
	WORD	ssize;			/* Bytes per sector (512, 1024, 2048 or 4096) */					//��ÿ�������ֽ���(������������512Byte��flash)��
#endif
#if _FS_REENTRANT
	_SYNC_t	sobj;			/* Identifier of sync object */										//���������룬������ͬ����������tiny�С�
#endif
#if !_FS_READONLY
	DWORD	last_clust;		/* Last allocated cluster */										//�����һ��������Ĵء�
	DWORD	free_clust;		/* Number of free clusters */										//�����дصĸ�����
	DWORD	fsi_sector;		/* fsinfo sector (FAT32) */											//�����fsinfo������(����FAT32)��
#endif
#if _FS_RPATH
	DWORD	cdir;			/* Current directory start cluster (0:root) */						//���������·��ʱ�ã��洢��ǰĿ¼��ʼ��(0:root)��
#endif
	DWORD	n_fatent;		/* Number of FAT entries (= number of clusters + 2) */				//��FAT�����(�ص���Ŀ + 2)��
	DWORD	fsize;			/* Sectors per FAT */												//��ÿ��FAT��ռ������
	DWORD	volbase;		/* Volume start sector */											//������ʼ������
	DWORD	fatbase;		/* FAT start sector */												//��FAT��ʼ������
	DWORD	dirbase;		/* Root directory start sector (FAT32:Cluster#) */					//����Ŀ¼��ʼ����(FAT32:Cluster#)��
	DWORD	database;		/* Data start sector */												//������Ŀ¼��ʼ������
	DWORD	winsect;		/* Current sector appearing in the win[] */							//����ǰ�������д洢�������š�
	BYTE	win[_MAX_SS];	/* Disk access window for Directory, FAT (and Data on tiny cfg) */	//�������������桿
} FATFS;



/* File object structure (FIL) ���ļ�����ṹ��*/

typedef struct {
	FATFS*	fs;				/* Pointer to the related file system object (**do not change order**) */	//�����ڵ�fsָ�롿
	WORD	id;				/* Owner file system mount ID (**do not change order**) */					//�����ڵ�fs���ر�š�
	BYTE	flag;			/* File status flags */														//���ļ�״̬��	
	BYTE	pad1;																						//����֪�����壬Ҳδ������ʹ�á�
	DWORD	fptr;			/* File read/write pointer (0ed on file open) */							//���ļ���дָ�롿
	DWORD	fsize;			/* File size */																//���ļ���С��
	DWORD	sclust;			/* File data start cluster (0:no data cluster, always 0 when fsize is 0) */ //���ļ���ʼ��(fsize=0ʱΪ0)��
	DWORD	clust;			/* Current cluster of fpter */												//����ǰ�ء�
	DWORD	dsect;			/* Current data sector of fpter */											//����ǰ����������
#if !_FS_READONLY		
	DWORD	dir_sect;		/* Sector containing the directory entry */									//������Ŀ¼���������	
	BYTE*	dir_ptr;		/* Pointer to the directory entry in the window */			 				//��ָ���ڴ����е�Ŀ¼��Ŀ��
#endif
#if _USE_FASTSEEK
	DWORD*	cltbl;			/* Pointer to the cluster link map table (null on file open) */ 			//��ָ�������ӳ����ָ�롿
#endif
#if _FS_LOCK
	UINT	lockid;			/* File lock ID (index of file semaphore table Files[]) */					//���ļ���ID��
#endif
#if !_FS_TINY
	BYTE	buf[_MAX_SS];	/* File data read/write buffer */							 				//���ļ��е����ݵĶ�/д���塿
#endif
} FIL;



/* Directory object structure (DIR) ��Ŀ¼����Ľṹ��*/

typedef struct {
	FATFS*	fs;				/* Pointer to the owner file system object (**do not change order**) */	   //�����ڵ�fsָ�롿
	WORD	id;				/* Owner file system mount ID (**do not change order**) */				   //�����ڵ�fs���ر�š�
	WORD	index;			/* Current read/write index number */									   //����ǰ��д�����š�
	DWORD	sclust;			/* Table start cluster (0:Root dir) */									   //���ļ���������ʼ�ء�
	DWORD	clust;			/* Current cluster */													   //����ǰ�ء�
	DWORD	sect;			/* Current sector */													   //����ǰ������
	BYTE*	dir;			/* Pointer to the current SFN entry in the win[] */						   //�����������е�ǰSFN���ָ�룬SFN����δ֪���²��LFN���ƣ����ļ�����ء�
	BYTE*	fn;				/* Pointer to the SFN (in/out) {file[8],ext[3],status[1]} */			   //��ָ����ļ�����
#if _USE_LFN
	WCHAR*	lfn;			/* Pointer to the LFN working buffer */									   //��ָ���ļ������塿
	WORD	lfn_idx;		/* Last matched LFN index number (0xFFFF:No LFN) */						   //����
#endif
} DIR;



/* File status structure (FILINFO) ���ļ�״̬�ṹ��*/

typedef struct {
	DWORD	fsize;			/* File size */
	WORD	fdate;			/* Last modified date */
	WORD	ftime;			/* Last modified time */
	BYTE	fattrib;		/* Attribute */						//���ļ����ԡ�
	TCHAR	fname[13];		/* Short file name (8.3 format) */
#if _USE_LFN	//�����ļ���֧�֡�
	TCHAR*	lfname;			/* Pointer to the LFN buffer */
	UINT 	lfsize;			/* Size of LFN buffer in TCHAR */
#endif
} FILINFO;



/* File function return code (FRESULT) ���ļ������ķ����롿*/

typedef enum {
	FR_OK = 0,				/* (0) Succeeded */															//���ɹ���
	FR_DISK_ERR,			/* (1) A hard error occurred in the low level disk I/O layer */				//���ڵͼ���Ĵ���I / O�㷢��Ӳ����
	FR_INT_ERR,				/* (2) Assertion failed */													//������һ�������FAT�ṹ��һ���ڲ����󣬶����¸ú���ʧ�ܡ�
	FR_NOT_READY,			/* (3) The physical drive cannot work */									//��������������û�д洢���ʻ��κ�����ԭ�򣬶����´����������޷�������
	FR_NO_FILE,				/* (4) Could not find the file */											//���޷��ҵ��ļ���
	FR_NO_PATH,				/* (5) Could not find the path */											//���޷��ҵ�·����
	FR_INVALID_NAME,		/* (6) The path name format is invalid */									//���ļ�����Ч��
	FR_DENIED,				/* (7) Access denied due to prohibited access or directory full */			//�����ʱ��ܾ������ڽ�ֹ���ʻ�Ŀ¼ȫ��
	FR_EXIST,				/* (8) Access denied due to prohibited access */							//�����ļ��Ѵ��ڡ�
	FR_INVALID_OBJECT,		/* (9) The file/directory object is invalid */								//���ļ�/Ŀ¼�Ķ�������Ч�ġ�
	FR_WRITE_PROTECTED,		/* (10) The physical drive is write protected */							//��������������д������
	FR_INVALID_DRIVE,		/* (11) The logical drive number is invalid */								//���߼�����������Ч��
	FR_NOT_ENABLED,			/* (12) The volume has no work area */										//������û�й�������
	FR_NO_FILESYSTEM,		/* (13) There is no valid FAT volume */										//��û����Ч��FAT��
	FR_MKFS_ABORTED,		/* (14) The f_mkfs() aborted due to any parameter error */					//��������ֹ�κβ�������
	FR_TIMEOUT,				/* (15) Could not get a grant to access the volume within defined period */	//���޷���������Թ涨��ʱ���ڷ�������
	FR_LOCKED,				/* (16) The operation is rejected according to the file sharing policy */	//���ò����ǰ����ļ�������Ծܾ���
	FR_NOT_ENOUGH_CORE,		/* (17) LFN working buffer could not be allocated */						//��LFN�����������޷����䡿
	FR_TOO_MANY_OPEN_FILES,	/* (18) Number of open files > _FS_SHARE */									//���򿪵��ļ�����
	FR_INVALID_PARAMETER	/* (19) Given parameter is invalid */										//�������Ĳ�����Ч��
} FRESULT;



/*--------------------------------------------------------------*/
/* FatFs module application interface                           */

FRESULT f_mount (BYTE vol, FATFS* fs);								/* Mount/Unmount a logical drive */					//����FatFsģ����ע��/ע��һ��������(�ļ�ϵͳ����)��
FRESULT f_open (FIL* fp, const TCHAR* path, BYTE mode);				/* Open or create a file */							//������/��һ�����ڷ����ļ����ļ�����
FRESULT f_read (FIL* fp, void* buff, UINT btr, UINT* br);			/* Read data from a file */							//����һ���ļ���ȡ���ݡ�
FRESULT f_lseek (FIL* fp, DWORD ofs);								/* Move file pointer of a file object */			//���ƶ�һ���򿪵��ļ�������ļ���/дָ�롣Ҳ���Ա�������չ�ļ���С(��Ԥ����)����
FRESULT f_close (FIL* fp);											/* Close an open file object */						//���ر�һ���򿪵��ļ���
FRESULT f_opendir (DIR* dj, const TCHAR* path);						/* Open an existing directory */					//����һ��Ŀ¼��
FRESULT f_readdir (DIR* dj, FILINFO* fno);							/* Read a directory item */							//����ȡĿ¼�
FRESULT f_stat (const TCHAR* path, FILINFO* fno);					/* Get file status */								//����ȡ�ļ�״̬��
FRESULT f_write (FIL* fp, const void* buff, UINT btw, UINT* bw);	/* Write data to a file */							//��д�����ݵ�һ���ļ���
FRESULT f_getfree (const TCHAR* path, DWORD* nclst, FATFS** fatfs);	/* Get number of free clusters on the drive */ 		//����ȡ���дص���Ŀ��
FRESULT f_truncate (FIL* fp);										/* Truncate file */									//���ض��ļ���С��
FRESULT f_sync (FIL* fp);											/* Flush cached data of a writing file */			//����ϴһ��д�ļ��Ļ�����Ϣ��
FRESULT f_unlink (const TCHAR* path);								/* Delete an existing file or directory */			//���Ƴ�һ������
FRESULT	f_mkdir (const TCHAR* path);								/* Create a new directory */						//������һ��Ŀ¼��
FRESULT f_chmod (const TCHAR* path, BYTE value, BYTE mask);			/* Change attribute of the file/dir */				//���޸�һ���ļ���Ŀ¼�����ԡ�
FRESULT f_utime (const TCHAR* path, const FILINFO* fno);			/* Change times-tamp of the file/dir */				//�������޸�һ���ļ���Ŀ¼��ʱ�����
FRESULT f_rename (const TCHAR* path_old, const TCHAR* path_new);	/* Rename/Move a file or directory */				//��������һ������
FRESULT f_chdrive (BYTE drv);										/* Change current drive */							//����
FRESULT f_chdir (const TCHAR* path);								/* Change current directory */						//�������ı�һ���������ĵ�ǰĿ¼��
FRESULT f_getcwd (TCHAR* buff, UINT len);							/* Get current directory */							//����
FRESULT	f_getlabel (const TCHAR* path, TCHAR* label, DWORD* sn);	/* Get volume label */								//����
FRESULT	f_setlabel (const TCHAR* label);							/* Set volume label */								//����
FRESULT f_forward (FIL* fp, UINT(*func)(const BYTE*,UINT), UINT btf, UINT* bf);	/* Forward data to the stream */		//����ȡ�ļ����ݲ�����ת�����������豸��
FRESULT f_mkfs (BYTE vol, BYTE sfd, UINT au);						/* Create a file system on the drive */			    //�����������ϴ���һ���ļ�ϵͳ��
FRESULT	f_fdisk (BYTE pdrv, const DWORD szt[], void* work);			/* Divide a physical drive into some partitions */  //����
int f_putc (TCHAR c, FIL* fp);										/* Put a character to the file */					//���������ļ���д��һ���ַ���
int f_puts (const TCHAR* str, FIL* cp);								/* Put a string to the file */						//���������ļ���д��һ���ַ�����
int f_printf (FIL* fp, const TCHAR* str, ...);						/* Put a formatted string to the file */			//���������ļ���д��һ����ʽ���ַ�����
TCHAR* f_gets (TCHAR* buff, int len, FIL* fp);						/* Get a string from the file */					//�����ļ��ж�ȡһ���ַ�����

#define f_eof(fp) (((fp)->fptr == (fp)->fsize) ? 1 : 0)//�Ƚ��ļ��Ķ�дָ�����д�Ĵ�С�����ʱ����1������ȷ���0
#define f_error(fp) (((fp)->flag & FA__ERROR) ? 1 : 0)
#define f_tell(fp) ((fp)->fptr)
#define f_size(fp) ((fp)->fsize)

#ifndef EOF
#define EOF (-1)
#endif




/*--------------------------------------------------------------*/
/* Additional user defined functions                            */

/* RTC function */
#if !_FS_READONLY
DWORD get_fattime (void);
#endif

/* Unicode support functions */
#if _USE_LFN							/* Unicode - OEM code conversion */
WCHAR ff_convert (WCHAR chr, UINT dir);	/* OEM-Unicode bidirectional conversion */
WCHAR ff_wtoupper (WCHAR chr);			/* Unicode upper-case conversion */
#if _USE_LFN == 3						/* Memory functions */
void* ff_memalloc (UINT msize);			/* Allocate memory block */
void ff_memfree (void* mblock);			/* Free memory block */
#endif
#endif

/* Sync functions */
#if _FS_REENTRANT
int ff_cre_syncobj (BYTE vol, _SYNC_t* sobj);	/* Create a sync object */
int ff_req_grant (_SYNC_t sobj);				/* Lock sync object */
void ff_rel_grant (_SYNC_t sobj);				/* Unlock sync object */
int ff_del_syncobj (_SYNC_t sobj);				/* Delete a sync object */
#endif




/*--------------------------------------------------------------*/
/* Flags and offset address ����־��ƫ�Ƶ�ַ��                 */


/* File access control and file status flags (FIL.flag) ���ļ����ʿ��ƺ��ļ�״̬��־��*/

#define	FA_READ				0x01	//����ȡ���ݡ�
#define	FA_OPEN_EXISTING	0x00	//�����ļ�������ļ������ڣ����ʧ�ܡ���Ĭ�ϣ���
#define FA__ERROR			0x80	//����

#if !_FS_READONLY
#define	FA_WRITE			0x02	//��д�����ݡ�
#define	FA_CREATE_NEW		0x04	//������һ�����ļ�������ļ��Ѵ��ڣ��򴴽�ʧ�ܡ�
#define	FA_CREATE_ALWAYS	0x08	//������һ�����ļ�������ļ��Ѵ��ڣ����������ضϲ����ǡ�
#define	FA_OPEN_ALWAYS		0x10	//������ļ����ڣ���򿪣����򣬴���һ�����ļ���
#define FA__WRITTEN			0x20	//����
#define FA__DIRTY			0x40	//����
#endif


/* FAT sub type (FATFS.fs_type) */

#define FS_FAT12	1
#define FS_FAT16	2
#define FS_FAT32	3


/* File attribute bits for directory entry ���ļ����ԡ�*/

#define	AM_RDO	0x01	/* Read only */				//��ֻ����
#define	AM_HID	0x02	/* Hidden */				//�����ء�
#define	AM_SYS	0x04	/* System */				//��ϵͳ��
#define	AM_VOL	0x08	/* Volume label */			//����꣨��������
#define AM_LFN	0x0F	/* LFN entry */				//�����ļ���Ŀ��
#define AM_DIR	0x10	/* Directory */				//��Ŀ¼��
#define AM_ARC	0x20	/* Archive */				//���浵��
#define AM_MASK	0x3F	/* Mask of defined bits */	//����


/* Fast seek feature */
#define CREATE_LINKMAP	0xFFFFFFFF



/*--------------------------------*/
/* Multi-byte word access macros  */

#if _WORD_ACCESS == 1	/* Enable word access to the FAT structure */
#define	LD_WORD(ptr)		(WORD)(*(WORD*)(BYTE*)(ptr))
#define	LD_DWORD(ptr)		(DWORD)(*(DWORD*)(BYTE*)(ptr))
#define	ST_WORD(ptr,val)	*(WORD*)(BYTE*)(ptr)=(WORD)(val)
#define	ST_DWORD(ptr,val)	*(DWORD*)(BYTE*)(ptr)=(DWORD)(val)
#else					/* Use byte-by-byte access to the FAT structure */
#define	LD_WORD(ptr)		(WORD)(((WORD)*((BYTE*)(ptr)+1)<<8)|(WORD)*(BYTE*)(ptr))
#define	LD_DWORD(ptr)		(DWORD)(((DWORD)*((BYTE*)(ptr)+3)<<24)|((DWORD)*((BYTE*)(ptr)+2)<<16)|((WORD)*((BYTE*)(ptr)+1)<<8)|*(BYTE*)(ptr))
#define	ST_WORD(ptr,val)	*(BYTE*)(ptr)=(BYTE)(val); *((BYTE*)(ptr)+1)=(BYTE)((WORD)(val)>>8)
#define	ST_DWORD(ptr,val)	*(BYTE*)(ptr)=(BYTE)(val); *((BYTE*)(ptr)+1)=(BYTE)((WORD)(val)>>8); *((BYTE*)(ptr)+2)=(BYTE)((DWORD)(val)>>16); *((BYTE*)(ptr)+3)=(BYTE)((DWORD)(val)>>24)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _FATFS */
