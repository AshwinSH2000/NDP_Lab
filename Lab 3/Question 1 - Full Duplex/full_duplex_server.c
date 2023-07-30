#include<stdio.h> 
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<string.h>

#define myport 1234

int main()
{
	int acptfd = 0, size;
	char buffer[30];
	char reply[30];
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);	//socket creation
	struct sockaddr_in	server_addr, client_addr;	
	
	//feeding values into the socket address structure
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(myport);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset( server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero) );
	
	bind( sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr) );
	
	//ready for listening
	listen(sockfd, 10);
	printf("The server is ready for chatting.\n");
	size = sizeof(struct sockaddr);
	acptfd = accept( sockfd, (struct sockaddr *) &client_addr, &size);	//connection accepted but data yet to be received.
	int pid = fork();
	while(1)
	{
		if (pid == 0)
		{
			//if child, receive from client and display it
			recv(acptfd, buffer, 30, 0);
			printf("\nClient's message = %s\n", buffer );
		}
		else
		{
			//if parent, read from the user and send it to client
			printf("\nEnter your reply: ");
			fgets(reply, 30, stdin);
			if(strncmp(buffer, "BYE",3)==0)
				break;
			send(acptfd, reply, 30, 0);
		}	
		//scanf("%[^\n]s", reply );	

	}
	close(sockfd);
	printf("\nClosing the socket\n\n");
	return 0;
}



