#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <time.h>

#define PORT 9015
#define MAX 1024

struct QA{
	char question[1024];
	char choice[4][30];
	int answer;
};

void chatLoop(int clientSocket){
	struct QA recvMsg;
	int sendMsg;
	char next[10];
	int score = 0;
	char name[30];
	int timer = 0;
	time_t start, end;

	printf("\033[1;36m");
	printf("Enter your name: ");
	printf("\033[0m");
	fgets(name, sizeof(name), stdin);
	printf("\n");
	send(clientSocket, name, strlen(name), 0);

	for(int i = 0; i < 5; i++){
        recv(clientSocket, &recvMsg, sizeof(recvMsg), 0);

        printf("\033[1;36m");
        printf("Server: ");
        printf("\033[0m");
		printf("\n%s\n1)%s\n2)%s\n3)%s\n4)%s\n", recvMsg.question, recvMsg.choice[0], recvMsg.choice[1], recvMsg.choice[2], recvMsg.choice[3]);
        printf("\033[1;36m");
        printf("Client: ");
        printf("\033[0m");
		fflush(stdout);
		memset(&recvMsg, 0, sizeof(recvMsg));

		struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };
		time(&start);
    	if(poll(&mypoll, 1, 60000)){
			scanf("%d", &sendMsg);
			time(&end);
		}
		else
			sendMsg = -1;

		end -= start;
		timer += end;
		
        send(clientSocket, &sendMsg, sizeof(int), 0);
		if(sendMsg == -1){
			printf("\033[1;31m");
			printf("\nTimeout..\nBetter luck next time..!\nScore: %d/50\nDisconnected from the server.\n\n", score * 10);
			printf("\033[0m");
			exit(EXIT_FAILURE);
		}

		recv(clientSocket, next, sizeof(next), 0);
        if(strncmp(next, "lost", 4) == 0){
			printf("\033[1;31m");
			printf("\nWrong answer.\nBetter luck next time..!\nScore: %d/50\nDisconnected from the server.\n\n", score * 10);
			printf("\033[0m");
			exit(EXIT_FAILURE);
		}
		memset(next, 0, sizeof(next));
		score++;
		printf("\n");
		sleep(2);
    }
	printf("Waiting for the results...\n");
	send(clientSocket, &timer, sizeof(int), 0);
	recv(clientSocket, next, sizeof(next), 0);
	if(strncmp(next, "lose", 4) == 0){
		printf("\033[1;31m");
		printf("\nBetter luck next time.\nScore: %d/50\nDisconnected from the server.\n\n", score * 10);
		printf("\033[0m");
		exit(EXIT_FAILURE);
	}
	printf("\033[1;32m");
	printf("\nWinner!\nScore: %d/50\nDisconnected from the server.\n\n", score * 10);
	printf("\033[0m");
	exit(EXIT_SUCCESS);
}

int main(){
	time_t t;
	time(&t);
	printf("\033[1;33m");
	printf("%s\n", ctime(&t));
	printf("\033[0m");

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
