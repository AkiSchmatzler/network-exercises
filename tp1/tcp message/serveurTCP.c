/**
* \author Aki Schmatzler
* \brief server for a very simple tcp message sender
* \date 10/10/21
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>


#define BACKLOG 5
#define BUF_SIZE 1024

void report(char* src) {
	fprintf(stderr, "%s : %s\nerror code : %d\n", src, strerror(errno), errno); 
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {

	if (argc != 2) {
		fprintf (stderr, "usage : ./serveurTCP <num_port>\n");
		exit(EXIT_FAILURE);
	}
	
	int s;	//socket descriptor
	//creating socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		report("socket");
	else fprintf(stdout, "socket created\n");

	//adress we're listening on
	struct sockaddr_in my_addr = {
		AF_INET,
		htons(atoi(argv[1])),
		htonl(INADDR_ANY)
	};

	//binding socket to port argv[1]
	if (bind(s, (struct sockaddr*) &my_addr, sizeof(my_addr)) == -1) 
		report("bind");
	else fprintf(stdout, "socket binded\n");

	//preparing to accept connection requests
	if (listen(s, BACKLOG) == -1)
		report("listen");
	else fprintf(stdout, "listening...\n");
	
	//accepting incoming connection requests
	int new_connection;
	struct sockaddr_in client;
	socklen_t size_client = sizeof(client);

	if ((new_connection = accept(s, (struct sockaddr*) &client, &size_client)) == -1)
		report("accept");

	//getting address of sender
	char buf_addr[BUF_SIZE];
	if(inet_ntop(AF_INET, &client.sin_addr.s_addr, buf_addr, BUF_SIZE) <= 0) 
		report("inet_ntop");
	
	
	//print address and port of request
	fprintf(stdout, "connect request from : %s\nsent on port : %d\n", buf_addr, ntohs(client.sin_port));

	//receive msg

	int size = 0;
	char buf_msg[BUF_SIZE];
	memset(buf_msg, 0, BUF_SIZE);

	if ((size = recv(new_connection, buf_msg, BUF_SIZE, 0)) == -1)
		report("recv");
	


	fprintf(stdout, "\nnew message : %s\n", buf_msg);
	fprintf(stdout, "\n#   exiting and closing socket - thank u   #\n");

	if (close(new_connection) == -1) report("close");
	if (close(s) == -1) report("close");
	exit(EXIT_SUCCESS);
}