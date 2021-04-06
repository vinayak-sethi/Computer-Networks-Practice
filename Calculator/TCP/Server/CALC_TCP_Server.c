#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>

#define buffsize 250

int main()
{
	int server_socket,client_socket,sin_size;
	char buffer[buffsize];
	struct sockaddr_in server_address, client_address;
	int n,operation,num1,num2,result;

	//create a socket
	server_socket = socket(AF_INET,SOCK_STREAM,0);
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

	//listen to the incoming client request
	if(listen(server_socket, 10) == 0)
		printf("Listen successful\n");

	//accept a connection request from client
	sin_size = sizeof(struct sockaddr_in);
	if((client_socket = accept(server_socket, (struct sockaddr *)&client_address,&sin_size)) > 0)
		printf("Accept Successful\n");

	s:
		// Ask for number 1
		n = send(client_socket,"Enter number 1: ", strlen("Enter number 1 : "),0);
		recv(client_socket, &num1, sizeof(int),0); // read number 1	
		printf("Client - Number 1 is: %d \n", num1);

		// Ask for number 2
		n = send(client_socket,"Enter number 2: ", strlen("Enter number 2: "),0);
		recv(client_socket, &num2, sizeof(int),0); //read number 2
		printf("Client - Number 2 is: %d \n", num2);

		//Ask for choice
		n = send(client_socket,"Enter your choice : \n\t1. Addition\n\t2. Subtraction\n\t3. Multiplication
			\n\t4. Division\n\t5. Exit\n", strlen("\nEnter your choice : \n\t1. Addition\n\t2. Subtraction
			\n\t3. Multiplication\n\t4. Division\n\t5. Exit\n"),0);
		recv(client_socket, &operation,sizeof(int),0); //  read the choice
		printf("Client - Choosen operation is %d\n",operation);

		if(operation > 5)
		{
			printf("Invalid Operation\n");
			goto s;
		}

		//perform operation
		switch(operation)
		{
			case 1: result = num1 + num2;
					break;

			case 2: result = num1 - num2;
					break;

			case 3: result = num1 * num2;
					break;

			case 4: result = num1 / num2;
					break;

			case 5: goto q;
					break;
		}

		//show the answer
		send(client_socket, &result, sizeof(int),0);
		if(operation != 5)
			goto s;

	q:
		//close the socket
		close(client_socket);
		close(server_socket);

	return 0;
}