#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>

#define Max_Data_len 1024 

int main()
{
	int server_socket;
	char input[Max_Data_len];
	char output[Max_Data_len];
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
		printf("\nCould not bind to Client\n");
		exit(EXIT_FAILURE);
	}


	int a,n;
	socklen_t length = sizeof(server_address);

	printf("\nWaiting for Client Response\n");

	while(1)
	{
		//receive data from Client
		n = recvfrom(server_socket, (char *)output, sizeof(output), 0, (struct sockaddr *)&server_address, &length);
		output[n] = '\0';
		printf("\nClient : %s",output);
		if(output[0] == 'q' && output[1] == 'u' && output[2] == 'i' && output[3] == 't')
            break;
        
		//send data to Client
		printf("Server : ");
		fgets(input,sizeof(input),stdin);
		sendto(server_socket,(const char *)input, sizeof(input), 0 , (struct sockaddr *)&server_address, sizeof(server_address));
	}		

	//close the socket
	close(server_socket);

	return 0;
}