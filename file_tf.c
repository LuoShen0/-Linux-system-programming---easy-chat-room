#include "common.h"
#include "file_tf.h"

void send_file(int fd1)  //发送文件(fd1为管道文件)
{
	
	int fd3 = open("1.jpg", O_RDONLY); //打开要发送的文件
	if(fd3 == -1)
	{
		perror("打开源文件失败");
		exit(0);
	}

	char *buf = calloc(1, 1000);
	while(1)
	{
		int n = read(fd3, buf, 1000);

		if(n == 0)
		{
			printf("传输完毕！\n");
			break;
		}

		if(n == -1)
		{
			perror("读取源文件失败");
			printf("读取1.jpg失败: %s\n",strerror(errno));
			break;
		}

		write(fd1, buf, n);   //从buf写到管道1
	}

	free(buf);
	close(fd3);
}


void rev_file(int fd1)     //接收文件
{
	int tmp=0;
	printf("有未接收文件，是否接收，接收请按6！\n");
	scanf("%d",&tmp);
	if(tmp=6)
	{
				int fd4 = open("11.jpg", O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if(fd4 == -1)
	{
		printf("打开11.jpg失败: %s\n",strerror(errno));
		exit(0);
	}
	char *buf = calloc(1, 1000);

	while(1)
	{
		int n=read(fd1,buf,1000);  //从管道fd1里读到buf

		if(n < 1000)
		{
			printf("接收完毕！\n");
			break;
		}

		if(n == -1)
		{
			perror("接收源文件失败");
			printf("接收11.jpg失败: %s\n",strerror(errno));
			break;
		}

		write(fd4,buf,n);    //从buf写到文件fd4
	}

	free(buf);
	close(fd4);
	}

}

