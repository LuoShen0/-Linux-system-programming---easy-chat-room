#ifndef __LINK_H
#define __LINK_H

//用户信息结构体
struct  people
{
	int  id;  //ID号 
	char name[20]; //用来保存人名
};

//定义一个链表的结构体 
struct list_people
{
	
struct  people  date;  //数据域 	
struct list_people *next;  //地址域	

};

//消息队列结构体
struct msgbuf
{
	long mtype;
	char mtext[100];

};
int talking(struct list_people *d,struct  people *p,int msgid);
struct list_people * find_people(struct list_people *head);

#endif