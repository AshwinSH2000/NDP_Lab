#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#define myport 1234

int main()
{
	int sockfd, acptfd;
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in dest_addr;
	
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(myport);
	dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //this has to do something with the local host
	memset(dest_addr.sin_zero, '\0', sizeof(dest_addr.sin_zero));
	
	//client doesnt have to bind. it has to connect to the server now
	
	connect(sockfd, (struct sockaddr *) &dest_addr, sizeof(struct sockaddr));
	char buffer[30] ;
	char reply[30];
	int pid = fork();
	while(1)
	{
		if(pid == 0)
		{
			//If the process is child read the message from the console and send it to the server. 
			printf("\nEnter your message:");
			//scanf("%[^\n]s", buffer);
			fgets(buffer, 30, stdin);
			if(strncmp(buffer, "BYE",3)==0)
				break;
			send(sockfd, buffer, 30,0);
		}
		else
		{
			//If the process is parent receive the message from the server and display it.
			recv(sockfd, reply, 30, 0);
			printf("\nServer's reply: %s", reply);
		}		
	}	
	int x;
	scanf("%d", &x);
	close(sockfd);
	return 0;
}



