#ifndef __MENU_H
#define __MENU_H	

#define Null 0

//�ṹ�����Ͷ��� 
struct MenuItem     
{
//	char range_max,range_min; //��ǰ��ʾ���ʼ���������
	char MenuCount; //�ṹ�������Ԫ�ظ���
//	char selected;//��ǰ��ѡ����
	char x,y;//��ʾ����λ��
	char *DisplayString;//��ǰLCD��ʾ����Ϣ
	void (*Subs)();  //ִ�еĺ�����ָ��. 
	struct MenuItem *Childrenms;  //ָ���ӽڵ��ָ�� 
	struct MenuItem *Parentms;  //ָ�򸸽ڵ��ָ�� 
}; 

//==================================== 
//�ṹ������ 
extern struct MenuItem m0_main[2];
extern struct MenuItem m1_USART[2]; 
extern struct MenuItem m1_Flash_Disk[2];

void Run(struct MenuItem *currentMenu);

#endif//__MENU_H
