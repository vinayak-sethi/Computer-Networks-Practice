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
	int server_socket;
	char operation[3], buffer[20], num1[buffsize], num2[buffsize], res[buffsize];
	struct sockaddr_in server_address, client_address;
	int n,a,b,choice,result;

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
	n = recvfrom(server_socket, buffer, sizeof(buffer),0,(struct sockaddr *)&client_address,&length);

	s:
		// Ask for number 1
		sendto(server_socket,"Enter number 1: ", strlen("Enter number 1: "),0,(struct sockaddr *)&client_address, sizeof(client_address));
		n = recvfrom(server_socket, num1, sizeof(num1),0,(struct sockaddr *)&client_address,&length); // read number 1	
		a = atoi(num1);
		printf("Client - Number 1 is: %d \n", a);

		// Ask for number 2
		sendto(server_socket,"Enter number 2: ", strlen("Enter number 2: "),0,(struct sockaddr *)&client_address,sizeof(client_address));
		n = recvfrom(server_socket, num2, sizeof(num2),0,(struct sockaddr *)&client_address,&length); //read number 2
		b = atoi(num2);
		printf("Client - Number 2 is: %d \n", b);

		//Ask for choice
		sendto(server_socket,"Enter your choice : \n\t1. Addition\n\t2. Subtraction\n\t3. Multiplication\n\t4. Division\n\t5. Exit\n", 
			strlen("\nEnter your choice : \n\t1. Addition\n\t2. Subtraction\n\t3. Multiplication\n\t4. Division\n\t5. Exit\n"),0,(struct sockaddr *)&client_address,sizeof(client_address));
		n = recvfrom(server_socket, operation,sizeof(operation),0,(struct sockaddr *)&client_address,&length); //  read the choice
		choice = atoi(operation);
		printf("Client - Choosen operation is %d\n",choice);

		if(choice > 5)
		{
			printf("Invalid Operation\n");
			goto s;
		}

		//perform operation
		switch(choice)
		{
			case 1: result = a + b;
					break;

			case 2: result = a - b;
					break;

			case 3: result = a * b;
					break;

			case 4: result = a / b;
					break;

			case 5: goto q;
					break;
		}

		//show the answer
		sprintf(res, "%d" ,result);
		sendto(server_socket, res, sizeof(res),0,(struct sockaddr *)&client_address,sizeof(client_address));

		if(choice != 5)
			goto s;

	q:
		//close the socket
		close(server_socket);

	return 0;
}
