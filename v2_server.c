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

int sockfd, newsockfd, clilen, n, mysock, conn_sock;
int parent_wrt();
int child_rcv();
pid_t pid;
char greeting[] = "Welcome to Freshnuts' Chat Server\n";
char disconnect[] = "DISCONNECT\n";
char buffer[MAX_BUF];
char message[200];
struct sockaddr_in serv_addr, cli_addr;
socklen_t socksize = sizeof(struct sockaddr_in);

int main(int argc, char *argv) {

  memset(&serv_addr, 0, sizeof(serv_addr)); //zero the structure
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(PORTNUM);

// Intro
  printf("\n==============================================================\n");
  printf("v2 is a P2P Chat for Linux/Unix.\n");
  printf("This program will allow you to chat directly\n"); 
  printf("to client/server without a middle man.\n");
  printf("\nCTRL + C  or type \"DISCONNECT\" to quit\n");
  printf("==============================================================\n");
// Create Socket
  mysock = socket(AF_INET, SOCK_STREAM, 0);
  if ( mysock < 0) {
	perror("mysock Error: ");
  } else {
	printf("Created Socket.\n");
  }

// Bind Socket
  if (bind(mysock,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
	perror("bind Error: ");
	exit(0);
  } else {
	printf("Binded Socket.\n");
  }

  if (listen(mysock, 1) < 0) {
	perror("Error: ");
	exit(0);
  }
  printf("Listening for Connections.\n");

// Accept Client
  conn_sock = accept(mysock, (struct sock_addr *)&cli_addr, &socksize);  
  if (conn_sock < 0) {
	perror("Cannot Accept Client");
  }

// Greeting
	printf("Incoming Connection from: %s\n", inet_ntoa(cli_addr.sin_addr));
	write(conn_sock, greeting, sizeof(greeting));
	recv(conn_sock, buffer, sizeof(buffer), 0);
	printf("%s #> %s\n", inet_ntoa(cli_addr.sin_addr), buffer);

// Chat Functions
  pid = fork();

  if (pid == 0 ) {
	child_rcv();
  } else if (pid > 0) {
	parent_wrt();
  } else {
	printf("Failed to fork\n");
  }

  close(mysock);
  return 0;
}

//Parent Process (Write to Socket Function)
int parent_wrt() {
  while (conn_sock) {
	fgets(message, sizeof(message), stdin);
    if (strcmp(message, disconnect) == 0) {
//	  write(conn_sock,disconnected, sizeof(disconnected));
      shutdown(conn_sock, 2);
	  exit(0);
    } else {
	  write(conn_sock, message, sizeof(message));
	  memset(message, 0, sizeof(message));
	}
  }
  return 0;
}
// Child Process (Recover from Socket Function)
int child_rcv() {
  while (conn_sock) {
	if (recv(conn_sock, buffer, sizeof(buffer), 0) <= 0) {
	  perror("Disconnected by Client: ");
	  shutdown(conn_sock, 2);
	  exit(0);
	} else {
	  printf("%s #> %s", inet_ntoa(cli_addr.sin_addr), buffer);
	  memset(buffer, 0, sizeof(buffer));
	}
  }
  return 0;
}
