#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<libgen.h>
#include<time.h>

#define buffsize 1024

int main()
{
	int client_socket,sin_size, check;
	char sendbuffer[buffsize];
	struct sockaddr_in server_address;

	//create a socket
	client_socket = socket(AF_INET,SOCK_STREAM,0);
	if(client_socket == -1)
	{
		printf("\nSocket Creation Failure\n");
		exit(EXIT_FAILURE);
	}

	//specify an address for the socket
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9009);
	server_address.sin_addr.s_addr = INADDR_ANY;

	sin_size = sizeof(struct sockaddr_in);

	//connect to server
	if(connect(client_socket,(struct sockaddr *)&server_address, sin_size) == 0)
		printf("Connect Successful\n");

	char path[buffsize];
	printf("Enter the complete path of the filename you wish to send : ");
	scanf("%s",path);
	char *filename = basename(path); //return the last component of a pathname
	printf("%s\n", filename);

	send(client_socket, filename, strlen(filename), 0);

	FILE *fp = fopen(path,"rb");
	if (fp == NULL)
	{
		printf("Cannot open the file\n");
		exit(EXIT_FAILURE);
	}

	clock_t t;
	t = clock();

	while((check = fread(sendbuffer, 1, sizeof(sendbuffer), fp)) > 0)
	{
		send(client_socket, sendbuffer, check, 0);
	}

	fclose(fp);
	t = clock() - t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC; //in seconds
	
	printf("File sent successfully ...\n");
	printf("File transfer took %f seconds to execute.\n", time_taken);
	
	close(client_socket);
	return 0;
}

