#include<stdio.h> 
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<string.h>
#include<arpa/inet.h>
#define myport 1234

#define MAXLINE 100
char result[100] = "\0";
int main()
{
	int connfd = 0, size, n;
	char buffer[30];
	char reply[30];
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);	//socket creation
	struct sockaddr_in	server_addr, client_addr;	


	//feeding values into the socket address structure
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(myport);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset( server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero) );
	
	bind( listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr) );
	
	//ready for listening	
	listen(listenfd, 10);
	size = sizeof(client_addr);
	pid_t childpid;
	char buf[100] = "\0";
	int count = 0;
	FILE *fp, *fo;
	fp = fopen("text.txt", "w");
	fputs("Manipal ", fp);
	fclose(fp);
	char res[100];
	int flag = 0;

	while(1)
	{
		connfd=accept(listenfd,(struct sockaddr* ) &client_addr , &size) ;
		printf("\n\n%s","Received request..." ) ;
		count++;
		childpid = fork();
		if(childpid==0 ) 
		{
			printf ("\n%s" , "Child created for dealing with client requests" ) ;
			close ( listenfd );		
			recv( connfd , buf , MAXLINE , 0 );
			//strcat(result, buf);
			//printf("\nConcatenated string: %s\n", result);
			fp = fopen("text.txt", "a");
			fputs(buf, fp);
			fclose(fp);
			char *ip;
			ip = inet_ntoa(client_addr.sin_addr);
			printf("\nClient ip address = %s\n", ip);
			if(count == 2)
			{
				fo = fopen("text.txt", "r");
				while((fgets(buf, 50, fo)!=NULL))
					strcat(result, buf);
				printf("\nConcatenated string: %s\n", result);
			}			
			close(connfd);
			exit(0);
		}
		else
		{
			if(count>2)
			{
				flag = 1;
				strcpy(res, "terminate");
				send(connfd, res, sizeof(res), 0);
				break;
			}
			close(connfd) ;			
		}
	}
}



