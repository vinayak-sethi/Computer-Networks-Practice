#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<libgen.h>
#include<sys/time.h>

#define buffsize 1024

int main()
{
	int client_socket,check,size = 0;
	struct timeval start, end;
	struct sockaddr_in server_address;

	//create a socket
	client_socket = socket(AF_INET,SOCK_DGRAM,0);
	if(client_socket == -1)
	{
		printf("\nSocket Creation Failure\n");
		exit(EXIT_FAILURE);
	}

	//specify an address for the socket
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9009);
	server_address.sin_addr.s_addr = INADDR_ANY;

	socklen_t length = sizeof(server_address);

	sendto(client_socket,"Hello server", strlen("Hello server"),0,(struct sockaddr *)&server_address, sizeof(server_address));

	char path[buffsize];
	printf("Enter the complete path of the filename you wish to send : ");
	scanf("%s",path);
	char *filename = basename(path); //return the last component of a pathname
	printf("%s\n", filename);

	sendto(client_socket, filename, strlen(filename), 0,(struct sockaddr *)&server_address, sizeof(server_address));

	FILE *fp = fopen(path,"rb");
	if (fp == NULL)
	{
		printf("Cannot open the file\n");
		exit(EXIT_FAILURE);
	}

	gettimeofday(&start, NULL);

	while((check = fgetc(fp)) != EOF)
	{	
		size++;
		sendto(client_socket, &check, sizeof(check), 0,(struct sockaddr *)&server_address, sizeof(server_address));
		sleep(0);
	}

	//send the EOF to signal file end
	size++;
	sendto(client_socket, &check, sizeof(check), 0,(struct sockaddr *)&server_address, sizeof(server_address));

	gettimeofday(&end, NULL);
	double duration = (double)(end.tv_usec - start.tv_usec) / 1000000 + (double)(end.tv_sec - start.tv_sec);

	fclose(fp);
	
	printf("File sent successfully ...\n");
	printf("Transferred file size: %f MiB\n", (float) size / 1000000);
	printf("Time taken to transfer the file: %f seconds.\n", duration);
	
	close(client_socket);
	return 0;
}
