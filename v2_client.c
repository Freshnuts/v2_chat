#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAX_BUF 256
#define PORTNUM 4441

int parent_wrt();
int child_wrt();

pid_t pid;
int *accepted = "Client has Connected";
char disconnect[] = "DISCONNECT\n";
char disconnected[] = "Client Disconnected, Press CTRL + C\n";
int sockfd, mysock, connected;
struct sockaddr_in server;
char buffer[255];
char message[255];

int main(int argc, char *argv[]) {

  memset(&server, 0, sizeof(server)); // zero the structure
  server.sin_family = AF_INET;
  server.sin_port = htons(PORTNUM);
  server.sin_addr.s_addr = INADDR_ANY;

// Intro
  printf("\n==============================================================\n");
  printf("v2 is a P2P Chat for Linux/Unix.\n");
  printf("This program will allow you to chat directly\n");
  printf("to client/server without a middle man.\n");
  printf("\nCTRL + C  or type \"DISCONNECT\" to quit\n");
  printf("==============================================================\n");

// Create Socket
  mysock = socket(AF_INET, SOCK_STREAM, 0);
	if (mysock < 0) { 
	  perror("Cannot Create Socket: \n");
	  exit(0);
	}
  printf("Created Socket.\n");

  connected = connect(mysock, &server, sizeof(server));
  if (connected < 0) {
	perror("Cannot connect to server: \n");
	exit(0);
  }
  printf("Connected to Server.\n");

// Greeting 
  if (recv(mysock, buffer, MAX_BUF, 0) < 0) {
	perror("Error: ");
  } else {
    printf("%s #> %s", inet_ntoa(server.sin_addr), buffer);
  }
// Client Reply
  if (write(mysock, accepted, sizeof(buffer)) < 0) {
	perror("Cannot send reply: \n");
	close(mysock);
	exit(0);
    }

// Chat Loop
  pid = fork();

  if (pid == 0) {
	child_rcv();
  } else if (pid > 0) {
	parent_wrt();
  } else {
	printf("Failed to fork.");
  }
  close(mysock);
  return 0;
}


// Parent Process (Write to Socket function)
int parent_wrt() {
  while (mysock) {
	fgets(message, sizeof(message), stdin);
    if (strcmp(message, disconnect) == 0) {
//    write(mysock, disconnected, sizeof(disconnected));
      shutdown(mysock, 2);
      exit(0);
      } else {
	  write(mysock, message, sizeof(message));
      memset(message, 0, sizeof(message));
	}
  }
  return 0;
}
// Child Process (Recover from socket function)
int child_rcv() {
  while (mysock) {
    if (recv(mysock, buffer, MAX_BUF, 0) <= 0) {
	  perror("Disconnected by Server ");
	  shutdown(mysock, 2);
	  exit(0);
	} else {
	  printf("%s #> %s", inet_ntoa(server.sin_addr), buffer);
      memset(buffer, 0, sizeof(buffer));
	  // printf("Enter Msg2: ");
	  // fflush(stdout);
	}
  }
  return 0;
}
