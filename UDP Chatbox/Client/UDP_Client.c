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
	int client_socket;
	char input[Max_Data_len];
	char output[Max_Data_len];
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

	int a,n;
	socklen_t length = sizeof(server_address);

	while(1)
	{
		//send data to server
		printf("Client : ");
		fgets(input,sizeof(input),stdin);
		if(input[0] == 'q' && input[1] == 'u' && input[2] == 'i' && input[3] == 't')
            break;

		sendto(client_socket,(const char *)input, sizeof(input), 0 , (struct sockaddr *)&server_address, sizeof(server_address));

		//receive data from server
		n = recvfrom(client_socket, (char *)output, sizeof(output), 0, (struct sockaddr *)&server_address, &length);
		output[n] = '\0';
		printf("Server : %s\n",output);
	}		

	//close the socket
	close(client_socket);

	return 0;
}
