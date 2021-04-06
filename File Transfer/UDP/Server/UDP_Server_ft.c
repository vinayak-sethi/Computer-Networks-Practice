#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<time.h>

#define buffsize 1024

int main()
{
	int server_socket,check,store;
	char buffer[buffsize];
	struct sockaddr_in server_address, client_address;

	//create a socket
	server_socket = socket(AF_INET,SOCK_DGRAM,0);
	if(server_socket == -1)
	{
		printf("\nSocket Creation Failure\n");
		exit(EXIT_FAILURE);
	}

	//specify an address for the socket
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(9009);
	server_address.sin_addr.s_addr = INADDR_ANY;

	//bind with the client
	if( bind(server_socket, (const struct sockaddr *)&server_address, sizeof(server_address)) < 0)
	{
		printf("Could not bind to Client\n");
		exit(EXIT_FAILURE);
	}

	socklen_t length = sizeof(server_address);
	recvfrom(server_socket, buffer, sizeof(buffer),0,(struct sockaddr *)&client_address,&length);

	char filename[buffsize];
	memset (filename, '\0', sizeof(filename));
	recvfrom(server_socket, filename, sizeof(filename), 0,(struct sockaddr *)&client_address,&length);
	printf("File received is : ");
	printf("%s\n", filename);

	clock_t t;
	t = clock();

	FILE *fp = fopen(filename, "wb");
	if(fp!=NULL)
	{	
		while(store != EOF)
		{	
			recvfrom(server_socket, &store, sizeof(store), 0,(struct sockaddr *)&client_address,&length);
			fputc(store, fp);		
		}
	}

	fclose(fp);
	t = clock() - t;
	double time_taken = ((double)t)/CLOCKS_PER_SEC; //in seconds

	printf("File received successfully...\n");
	printf("File received in %f seconds.\n", time_taken);
	
	//close the socket
	close(server_socket);
	return 0;
}