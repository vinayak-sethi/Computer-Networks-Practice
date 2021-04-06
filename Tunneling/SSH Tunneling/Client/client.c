#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 9001
#define MAX 1024

int main(){
	int clientSocket;
	char recvMsg[MAX];
	char sendMsg[MAX];
	struct sockaddr_in clientAddr;

	if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("Socket could not be created.\n");
		exit(EXIT_FAILURE);
	}

	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(PORT);
	clientAddr.sin_addr.s_addr = INADDR_ANY;

	int n;
    socklen_t len = sizeof(clientAddr);

    connect(clientSocket, (struct sockaddr *)&clientAddr, len);

	while(1){
        printf("Client: ");
        fgets(sendMsg, sizeof(sendMsg), stdin);
        send(clientSocket, (char *)sendMsg, sizeof(sendMsg), 0);
        if(sendMsg[0] == 'q' && sendMsg[1] == 'u' && sendMsg[2] == 'i' && sendMsg[3] == 't')
            break;

        n = recv(clientSocket, (char *)recvMsg, sizeof(recvMsg), 0);
        recvMsg[n] = '\0';
        printf("Server: %s\n", recvMsg);
    }
	close(clientSocket);
	return 0;
}