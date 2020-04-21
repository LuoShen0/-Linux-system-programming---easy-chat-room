#include "common.h"
#include "file_tf.h"
#include "talking.h"
#include <signal.h>
//插入节点  
void inser_node(struct list_people *head,struct people *date)
{
	
	  //分配节点空间  
	  struct list_people *new = malloc(sizeof(struct list_people));
	
	 //为节点赋值  
	 new->date.id = date->id;  
	 strcpy(new->date.name,date->name);
	 new->next = NULL;
	
	 //遍历链表找到最后一个节点 
	 struct list_people *tmp = head;  
	 
	 while(tmp->next != NULL)
	 {
		tmp=tmp->next;
	 }
	 
	 //连接节点
	 tmp->next = new; 
	
}

//显示链表上的好友
void show_list_people(struct list_people *head)
{
    //指向第一个节点
	struct list_people *p = head->next;
	printf("当前在线好友列表：\n");
	while(p != NULL)
	{
		
		printf("ID：%d\t姓名:%s\n",p->date.id,p->date.name);
		
		 p = p->next;
	}
}

//用户退出
void exit_people(struct  people *p,int id)
{
	while(p->id != id)  //找到自己的ID号
	{
		p++; 
	}
	
	//把该位置清空  曾经有人用过这个位置
	p->id = 1;
	
	exit(0);
}

//销毁链表  
void del_list(struct list_people *head)
{
	
	while(1)
	{
		
		if(head->next == NULL)
		{
			break;
		}
		
		struct list_people *p = head->next;  
		if(p!=NULL)
		{
			
			head->next = p->next; 
			free(p);
		}
	}
	
}

//更新好友列表
void update_people(struct list_people *head,struct  people *p)
{
	//销毁链表
	del_list(head);

	while(p->id != 0)
	{
		//把共享内存中的信息加入到链表中
		if(p->id != 1)
		{
			inser_node(head,p);
		}
		
		p++;   //找到0 的位置  没有数据的位置 
	}		
}

int main(int argc,char **argv)
{
	
	if(argc != 2 )
	{
		printf("请输入用户名\n");
		return -1;
	}
	
	 //分配一个头节点 
	 struct list_people *head = malloc(sizeof(struct list_people));
	 bzero(head,sizeof(struct list_people));
	 head->next = NULL;
	
	
	 struct  people  *p = NULL;
	
	//获取密钥 
    key_t  ket=ftok("/home/gec",666);

	//通过key 创建共享内存  
     int  shmid=shmget(ket,sizeof(struct  people)*100,IPC_CREAT|IPC_EXCL|0777);
		 if(shmid < 0)
		 {
			 //处理错误宏  存在 则直接打开
			 if(errno  == EEXIST )
			 {
				shmid=shmget(ket,sizeof(struct  people)*100,IPC_CREAT); 
				//映射
				   p = shmat(shmid,NULL,0);
					if(p == (void *) -1)
					{
						perror("");
						return -1;
					} 
			 }
		 }
		 else
		 {
		    //把共享内存映射到用户空间中 
            p = shmat(shmid,NULL,0);
			if(p == (void *) -1)
			{
				perror("");
				return -1;
			}
			
			memset(p,0,sizeof(struct  people)*100);
			
		 }

	//保存映射后的位置
	struct  people  *tmp = p;

	int biaozhi=0;
	
	//找到合适的位置插入信息
	while(p->id != 0)
	{
	
		if(p-> id == 1)  //找到插入的位置
		{
			break; 
		}
		
		p++;   //找到0 的位置  没有数据的位置 
	}
	//往该位置写入数据； 
	p->id = getpid();   //获取进程的PID  
	strcpy(p->name,argv[1]);
	printf("当前用户id:%d,姓名:%s\n",p->id,p->name);
	
	
	//更新好友列表 
	update_people(head,tmp);


	//建立消息队列
	key_t key1=ftok("/home/eva",01);
	int msgid=msgget(key1,IPC_CREAT|0777);

	//通过管道传输文件
	//打开管道
	int  fd1 = open("./pi1",O_RDWR);
		if(fd1 < 0)
		{
			perror("");
			return -1; 
		}

	while(1)
	{	
		printf("-----------------------------------操作选项-------------------------------\n");
		printf("输入1.显示好友列表  2.选择一个好友  3.更新好友列表  0.退出当前用户\n");
		printf("    4.进入聊天      5.发送文件      6.查看是否有未接收文件文件\n");

		int a=0;
		scanf("%d",&a);
		struct list_people *c;
		if(a == 1)
		{
			show_list_people(head);	//显示链表中的好友
		}
		else if(a == 2)  
		{
			c=find_people(head); //找到指定好友
		}
		else if(a == 3)  
		{
			update_people(head,tmp);//更新好友列表
		}
		else if(a==4)    
		{
			talking(c,p,msgid);//进入聊天
		}	
		else if(a==5)   
		{
			send_file(fd1); //发送文件
		}
		else if(a==6)   
		{
			rev_file(fd1); //接收文件
		}
		else if(a==0)   
		{
			exit_people(tmp,getpid());  //退出当前用户
		}		
	}

	return 0;
		
}