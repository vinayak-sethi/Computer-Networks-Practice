#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 9009
#define MAX 1024

int main() {
	int serverSocket, clientSocket;
	char recvMsg[MAX];
	char sendMsg[MAX];
	struct sockaddr_in serverAddr, clientAddr;
	
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Socket could not be created.\n");
		exit(EXIT_FAILURE);
	}
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);
	
	if(bind(serverSocket, (const struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
		printf("Could not bind with the client.\n");
		exit(EXIT_FAILURE);
	}
	
	int n;
    socklen_t len = sizeof(serverAddr);

    printf("Waiting for a client to connect......\n\n");

    listen(serverSocket, 10);
    if((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &len)) < 0){
        printf("Could not connect to the client.\n");
        exit(EXIT_FAILURE);
    }

    printf("Client connected. Waiting for a message.......\n\n");

    while(1){
        n = recv(clientSocket, (char *)recvMsg, sizeof(recvMsg), 0);
        if(recvMsg[0] == 'q' && recvMsg[1] == 'u' && recvMsg[2] == 'i' && recvMsg[3] == 't')
            break;
        recvMsg[n] = '\0';
        printf("Client: %s", recvMsg);
        printf("Server: ");
        fgets(sendMsg, sizeof(sendMsg), stdin);
        send(clientSocket, (char *)sendMsg, sizeof(sendMsg), 0);
        printf("\n");
	}

	close(serverSocket);
	return 0;
}