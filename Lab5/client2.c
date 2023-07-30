#include<stdio.h> 
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<string.h>
#include<arpa/inet.h>
#define myport 1234


int main()
{
	int acptfd = 0;
	char buffer[30];
	char reply[30];
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);	//socket creation
	struct sockaddr_in server_addr;	
	//feeding values into the socket address structure
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(myport);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset( server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero) );
	char buf[100] = " Technology";
	int size = sizeof(server_addr);
	connect(listenfd,(struct sockaddr* ) &server_addr , size) ;	
		
	send(listenfd , buf , sizeof(buf) , 0 ) ;
	recv(listenfd, buf, sizeof(buf), 0);
	printf("\n%s\n", buf);
}
