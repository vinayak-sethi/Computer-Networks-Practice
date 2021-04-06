#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>

#define buffsize 150

int main()
{
	int client_socket;
	char buffer[buffsize], operation[3];
	int n,result;
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

	s:
		//Read server message
        memset(&buffer,0,sizeof(buffer));
		n = recvfrom(client_socket,buffer,sizeof(buffer),0,(struct sockaddr *)&server_address,&length);
		printf("Server - %s\n",buffer);

		fgets(buffer, buffsize, stdin); // Enter number 1
		sendto(client_socket, buffer, sizeof(buffer),0,(struct sockaddr *)&server_address,sizeof(server_address)); // send number 1

		//Read server message
        memset(&buffer,0,sizeof(buffer));
		n = recvfrom(client_socket,(char *)buffer,sizeof(buffer),0,(struct sockaddr *)&server_address,&length);
		printf("Server - %s\n",buffer);

		fgets(buffer, buffsize, stdin); // Enter number 2
		sendto(client_socket, buffer, sizeof(buffer),0,(struct sockaddr *)&server_address,sizeof(server_address)); // send number 2

		//Read server message
        memset(&buffer,0,sizeof(buffer));
		n = recvfrom(client_socket,buffer,sizeof(buffer),0,(struct sockaddr *)&server_address,&length);
		printf("Server - %s\n",buffer);

		fgets(buffer, buffsize, stdin); // Enter choice
        strcpy(operation , buffer);
		sendto(client_socket, buffer, sizeof(buffer),0,(struct sockaddr *)&server_address,sizeof(server_address)); // send choice

		if(operation[0] == '5')
			goto q;
		
		//read the result
        memset(&buffer,0,sizeof(buffer));
		n = recvfrom(client_socket,buffer, sizeof(buffer),0,(struct sockaddr *)&server_address,&length);
		result = atoi(buffer);
		printf("Server - Your answer is %d\n",result);

		if(operation[0] != '5')
			goto s;

	q:
		//close the socket
		printf("You have selected to exit\n");
		close(client_socket);

	return 0;
}
