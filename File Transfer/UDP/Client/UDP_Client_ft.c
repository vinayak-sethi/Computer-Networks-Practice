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
	int client_socket,check;
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

	clock_t t;
	t = clock();

	while((check = fgetc(fp)) != EOF)
	{	
		sendto(client_socket, &check, sizeof(check), 0,(struct sockaddr *)&server_address, sizeof(server_address));
		sleep(0);
	}

	//send the EOF to signal file end
	sendto(client_socket, &check, sizeof(check), 0,(struct sockaddr *)&server_address, sizeof(server_address));

	fclose(fp);
	t = clock() - t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC; //in seconds

	printf("File sent successfully ...\n");
	printf("File transfer took %f seconds to execute.\n", time_taken);

	close(client_socket);
	return 0;
}