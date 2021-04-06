#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>

int main()
{
	int client_socket,sin_size;
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

	printf("****************************STOP AND WAIT SENDER SIDE***************************\n");

	char message[150];
	printf("\nEnter the message to send: ");
	gets(message);

	char buffer[2]; //to send the data packet
	buffer[0] = '1';
	buffer[1] = '\0'; 

	char acknowledge[2]; //successful acknowledgement received or not
	acknowledge[0] = '1'; 
	acknowledge[1] = '\0'; 

	char reached_stringend[2]; //reached end of string or not
	reached_stringend[0] = '0'; 
	reached_stringend[1] = '\0'; 

	//sending packet byte by byte
	for(int i=0; i<strlen(message); i++)
	{
		printf("\nSending data -> %c", message[i]);
		buffer[0] = message[i];
		reached_stringend[0] = '0';

		send(client_socket, reached_stringend, sizeof(reached_stringend), 0); //check if reached string end or not
		send(client_socket, buffer, sizeof(buffer), 0); //sending the data packet
		recv(client_socket, acknowledge, sizeof(acknowledge), 0); //receiving the acknowledgement
		printf("\nAcknowledgement -> %s\n", acknowledge);
		
		if(acknowledge[0] == '0') //Failed Acknowledgement
			i--;
	}

	reached_stringend[0] = '1'; //end of string
	send(client_socket, reached_stringend, sizeof(reached_stringend), 0);

	printf("Message sent succesfully..\n");
	printf("\n********************************************************************************\n");

	//close the socket
	close(client_socket);
	return 0;
}
