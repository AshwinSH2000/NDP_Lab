#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/types.h>
int search(char toberead[200], char str[10])
{
	int x = strlen(toberead);
	printf("\n\n%s\n%d\n", toberead, x);
    toberead[x] = ' ';
    toberead[x+1] = '\0';
    int i=0;
    char word[100];
    int wptr = 0;
    int found = 0;
    for(i = 0 ; toberead[i]!='\0' ; i++)
    {
        if(toberead[i] == ' ')
        {
            word[wptr] = '\0';
            wptr = 0;
            if(strcmp(word, str)==0)
           	   found ++;            	
        }
        else
            word[wptr++] = toberead[i];

    }
    printf("Finished executing the search function. returning");
    return found;
}
int replace(char replacetext[200], char toberead[200], char str[10], char str2[10])
{
    int x = strlen(toberead);
    printf("\n\n%s\n%d\n", toberead, x);
    toberead[x] = ' ';
    toberead[x+1] = '\0';
    int i=0;
    char word[100];
    int wptr = 0;
    int found = 0;
   
    for(i = 0 ; toberead[i]!='\0' ; i++)
    {
        if(toberead[i] == ' ')
        {
            word[wptr] = '\0';
            wptr = 0;
            if(strcmp(word, str)==0)
            {   found ++;
            	strcat(replacetext, str2);	
            	strcat(replacetext, " ");
            }
            else
            {
            	strcat(replacetext, word);
            	strcat(replacetext, " ");
            }
        }
        else
            word[wptr++] = toberead[i];
    }
    replacetext[strlen(replacetext)] = '\0';	
    return found;
}
int main()
{
	int sfd;
	sfd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in server, client;
	server.sin_family = AF_INET;
	server.sin_port = htons(1435);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&(server.sin_zero), '\0', sizeof(server.sin_zero));
	if (sfd == -1)
		printf("Error");
	else
		printf("Socket created successfully");
	
	bind(sfd, (struct sockaddr *)&server, sizeof(server));

	//printf("\nlistening");

	listen(sfd, 1);
	int size = sizeof(struct sockaddr);
	int afd = accept(sfd, (struct sockaddr *)& client, &size);
	char filename[20], str[10], toberead[200], str2[10];
	int result, ch;	
	//printf("Done");
	
	recv(afd, filename, sizeof(filename), 0);
	FILE *fp, *fo;
	fp = fopen(filename, "r");
	if(fp==NULL)
	{	result = 0;
		printf("Not present");
		send(afd, &result, sizeof(result), 0);
		close(fp);
		close(sfd);
		exit(0);
	}
	
	printf("Waiting for the choice");
	result = 1;
	int count = 0;	
	send(afd, &result, sizeof(result), 0);

	while(1){
	recv(afd, &ch, sizeof(ch), 0);
	//printf("received the choice");
	switch(ch)
	{
		case 1:printf("OPTION 1");
				fseek(fp, 0, SEEK_SET);

				recv(afd, str, sizeof(str), 0);
				printf("\n%s\n", str);
	
				count = 0;
				while( fgets(toberead, 100, fp)!=NULL )
					count += search(toberead, str);
				printf("DONEEEEEE");
				send(afd, &count, sizeof(count), 0);
	
				break;
		case 2:recv(afd, str, sizeof(str), 0);
				recv(afd, str2, sizeof(str2), 0);
				fseek(fp, 0, SEEK_SET);				
				fo = fopen("newfile.txt", "w");
				count = 0;
				char replacetext[200];
				while( fgets(toberead, 100, fp)!=NULL )
				{	count += replace(replacetext, toberead, str, str2);
					fputs(replacetext, fo);
				}
				if(count == 0)
				{
					char result[20] = "String not found";
					send(afd, result, sizeof(result), 0);
				}
				else
				{
					char result[20] = "String replaced";
					send(afd, result, sizeof(result), 0);
				}
				fclose(fo);
				
				break;
		case 3: fo = fopen("newfile.txt", "w");
				fseek(fp, 0, SEEK_SET);
				int j=0;
				char temp;				
				while( fgets(toberead, 100, fp)!=NULL )
				{
				for(int i=0 ; i<strlen(toberead)-1 ; i++)
					for(j=0 ; j<strlen(toberead)-i-1 ; j++)
						if(toberead[j]>toberead[j+1])
						{
							temp = toberead[j];
							toberead[j] = toberead[j+1];
							toberead[j+1] = temp;
						}
				fputs(toberead, fo);
				printf("\n%s", toberead);
				}
				char result[20] = {"sorted the file"};
				send(afd, result, sizeof(result), 0);
				//printf("%s" ,toberead);
				fclose(fo);
				break;
		case 4:printf("\nTerminating...\n");
				close(sfd);
				break;
				
	}
	if(ch == 4)
		break;
	}//while
	
	return 0;	
}
