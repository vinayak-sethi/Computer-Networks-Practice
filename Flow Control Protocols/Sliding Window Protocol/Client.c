#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<time.h>

double timeout = 3; //timeout time of acknowledgement

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

	printf("**************************SELECTIVE REPEAT SENDER SIDE**************************\n");

    int windowsize;
    printf("Enter the window size: ");
    scanf("%d", &windowsize);

    int win_size[] = {windowsize};
    send(client_socket, win_size, sizeof(win_size), 0); //send the window size to the receiving end

    char ch;
    scanf("%c", &ch);
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

    //sending continuous packet which can fit in a window
    for(int i=0; i<strlen(message); i++)
    {
        send(client_socket, reached_stringend, sizeof(reached_stringend), 0); //check if reached string end or not
        
        for(int j=0; j<windowsize; j++) //sending packets for a given window size
        {
            printf("\nSending data -> %c", message[i+j]);
		    buffer[0] = message[i+j];
            send(client_socket, buffer, sizeof(buffer), 0); //sending the data packet
        }

        int ack_check = -1;
        printf("\n");

        for(int j=0; j<windowsize; j++)
        {
			
            recv(client_socket, acknowledge, sizeof(acknowledge), 0); //receiving the acknowledgement
		    
			double time_taken[1]; //time taken by receiver to send the acknowledgement
			recv(client_socket, time_taken, sizeof(time_taken), 0);

			if(timeout < time_taken[0])
				printf("Acknowledgement -> TIMEOUT"); 
			else
				printf("Acknowledgement -> %s", acknowledge);

		    if(acknowledge[0] == '0' || timeout < time_taken[0]) //Failed Acknowledgement
            {
                ack_check = j;
                j = windowsize;
            }
            printf("\n");
        }

        if(ack_check == -1)
            i = i + windowsize - 1; // no failed acknowedgement received
        else
            i = i + ack_check - 1; //any failed acknowledgement received
    }

    reached_stringend[0] = '1'; //end of string
	send(client_socket, reached_stringend, sizeof(reached_stringend), 0);

    printf("\nMessage sent succesfully..\n");
	printf("\n********************************************************************************\n");

	//close the socket
	close(client_socket);
	return 0;
}