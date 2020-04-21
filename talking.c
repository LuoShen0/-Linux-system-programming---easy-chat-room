#include "common.h"
#include "talking.h"

int talking(struct list_people *d,struct  people *p,int msgid)
{
	
	pid_t x=fork();
	if(x>0)                    //父进程发送
	{
		while(1)
		{
			struct msgbuf send;
			send.mtype=d->date.id;
			printf("请输入与他的聊天内容：\n");
			scanf("%s",send.mtext);

			msgsnd(msgid,&send,strlen(send.mtext),0);
			break;
		}	
	}

	if(x==0)                    //子进程接收，消息类型为d的id
	{
		while(1)
		{

			struct msgbuf rec={0};			  
			msgrcv(msgid,&rec,sizeof(rec),p->id,0);
			
			printf("ID为：%d\n",d->date.id);
			printf("发来的消息内容为:%s\n",rec.mtext);
			//break;
	
		}
	}
	
}

//查找指定好友
struct list_people * find_people(struct list_people *head)
{
	struct list_people *b = head;  //b为要找的好友
	char buf[20];
	printf("请输入你要找的好友名字:\n");
	scanf("%s",buf);
	
	while(strcmp(b->date.name,buf)!=0)
	{
		b=b->next;
		if(b==NULL)
		{
			printf("该好友未上线!\n");
			exit(0);
		}
	}
	printf("已经找到该好友.其id为：%d\n",b->date.id);

	return b;
}