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
	int client_socket,sin_size;
	char buffer[buffsize];
	int n,operation,num1,num2,result;
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

	s:
		//Read server message
		memset(&buffer,0,sizeof(buffer));
		n = recv(client_socket,buffer,buffsize,0);
		printf("Server - %s\n",buffer);

		scanf("%d",&num1); // Enter number 1
		send(client_socket,&num1, sizeof(int),0); // send number 1

		//Read server message
		memset(&buffer,0,sizeof(buffer));
		n = recv(client_socket,buffer,buffsize,0);
		printf("Server - %s\n",buffer);

		scanf("%d",&num2); // Enter number 2
		send(client_socket,&num2, sizeof(int),0); // send number 2

		//Read server message
		memset(&buffer,0,sizeof(buffer));
		n = recv(client_socket,buffer,buffsize,0);
		printf("Server - %s",buffer);

		scanf("%d",&operation); // Enter choice
		send(client_socket,&operation, sizeof(int),0); // send choice

		if(operation == 5)
			goto q;
		
		//read the result
		recv(client_socket, &result, sizeof(int),0);
		printf("Server - Your answer is %d\n",result);

		if(operation != 5)
			goto s;

	q:
		//close the socket
		printf("You have selected to exit\n");
		close(client_socket);

	return 0;
}