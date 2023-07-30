#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>

int main()
{
	int sfd;
	sfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(1435);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&(server.sin_zero), '\0', sizeof(server.sin_zero));
	
	char filename[20];
	int result;
	int size = sizeof(struct sockaddr);
	connect(sfd, (struct sockaddr *)& server, size);
	printf("\nEnter the file name:");
	scanf("%s",filename);
	send(sfd, filename, sizeof(filename), 0);
	recv(sfd, &result, sizeof(result), 0);
	
	printf("\n\nrecved the result");
	if(!result)
	{
		printf("\nFile not present");
		close(sfd);
		exit(0);
	}
	printf("\n\nrecved the result222");
	while(1){
	printf("\n1. Search\n2. Replace\n3. Reorder\n4. Exit\nEnter your choice:");
	int ch;
	scanf("%d", &ch);
	//have to choose bw 4 options
	send(sfd, &ch, sizeof(ch), 0);
	char str[10], str2[10], res[20];
	int count;
	switch(ch)
	{
		case 1:printf("\nEnter the string to be searched:");
				scanf("%s",str);
				send(sfd, str, sizeof(str), 0);
				recv(sfd, &count, sizeof(count), 0);
				if(!count)
					printf("\nString not found");
				else
					printf("%s found %d times in the %s", str, count, filename);
				break;
		case 2:printf("\nEnter the string to be searched for:");
				scanf("%s",str);
				send(sfd, str, sizeof(str), 0);
				printf("\nEnter the string to replace:");
				scanf("%s",str2);
				send(sfd, str2, sizeof(str2), 0);
				recv(sfd, res, sizeof(res), 0);
				printf("%s\n", res);
				break;
		case 3:recv(sfd, res, sizeof(res), 0);
				printf("\n%s", res);
				break;
		case 4:printf("\nTerminating...");
				close(sfd);	
				break;
	}
	if(ch == 4)
		break;
	}//while
	
	return 0;	
}

