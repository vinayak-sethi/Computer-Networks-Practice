#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>

int main()
{
	int server_socket,client_socket,sin_size;
	struct sockaddr_in server_address, client_address;

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


	printf("***************************STOP AND WAIT RECEIVER SIDE**************************\n");
	char message[150];

	char acknowledge[2]; //successful acknowledgement sent or not
	acknowledge[0] = '1'; 
	acknowledge[1] = '\0';

	char reached_stringend[2], buffer[2];

	int message_len = 0;
	recv(client_socket, reached_stringend, sizeof(reached_stringend), 0);

	//Receiving Packet byte by byte
	while(reached_stringend[0] == '0')
	{
		recv(client_socket, buffer, sizeof(buffer), 0); //receiving packet

		printf("\nReceiving packet -> %s", buffer);
		printf("\nAcknowledgement [0/1] -> ");
		scanf("%s", acknowledge);

		send(client_socket, acknowledge, sizeof(acknowledge), 0); //send the acknowledgement corresponding to packet
		if(acknowledge[0] == '1') //Positive acknowlegment
			message[message_len++] = buffer[0];

		recv(client_socket, reached_stringend, sizeof(reached_stringend), 0);
		if(reached_stringend[0] == '1')
			break;
	}

	message[message_len++] = '\0'; //end of message
	printf("\nReceived message: ");
	for(int i=0; i<message_len; i++)
		printf("%c", message[i]);
	
	printf("\n");

	printf("\n********************************************************************************\n");

	//close the socket
	close(client_socket);
	close(server_socket);

	return 0;
}