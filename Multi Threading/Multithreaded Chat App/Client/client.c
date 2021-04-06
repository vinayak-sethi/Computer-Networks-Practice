#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 9005
#define MAX 1024

void chatLoop(int clientSocket){
	char recvMsg[MAX];
	char sendMsg[MAX];
	while(1){
        printf("\033[1;36m");
        printf("Client: ");
        printf("\033[0m");

        fgets(sendMsg, sizeof(sendMsg), stdin);
        send(clientSocket, (char *)sendMsg, sizeof(sendMsg), 0);

        if(strncmp(sendMsg, "quit", 4) == 0){
			printf("\033[1;31m");
			printf("\nDisconnected from the server.\n");
			printf("\033[0m");
			exit(EXIT_FAILURE);
		}

        int n = recv(clientSocket, (char *)recvMsg, sizeof(recvMsg), 0);
        recvMsg[n] = '\0';

        printf("\033[1;36m");
        printf("Server: ");
        printf("\033[0m");
		printf("%s\n", recvMsg);
    }
}

int main(){
	int clientSocket;
	struct sockaddr_in clientAddr;

	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\033[1;31m");
		printf("Socket could not be created.\n");
		printf("\033[0m");
		exit(EXIT_FAILURE);
	}

	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(PORT);
	clientAddr.sin_addr.s_addr = INADDR_ANY;

	int n;
    socklen_t len = sizeof(clientAddr);

    if(connect(clientSocket, (struct sockaddr *)&clientAddr, len) < 0){
        printf("\033[1;31m");
		printf("Could not connect to the server.\n");
        printf("\033[0m");
		exit(EXIT_FAILURE);
	}

	chatLoop(clientSocket);

	close(clientSocket);
	return 0;
}