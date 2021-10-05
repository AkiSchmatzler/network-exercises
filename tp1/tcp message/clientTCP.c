#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>




void report(char* src) {
	fprintf(stderr, "%s : %s\n", src, strerror(errno)); 
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {

	if (argc != 4) {
		fprintf (stderr, "usage : ./clientTCP <adress IP dist> <num port_dist> <message>\n");
		exit(EXIT_FAILURE);
	}

	char* message = argv[3];

	
	int s;	//socket descriptor
	//creating socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		report("socket");

	//adress we wanna connect to
	struct sockaddr_in addr_server;
	addr_server.sin_family = AF_INET;
	addr_server.sin_port = htons(atoi(argv[2]));

	//getting adress and converting in
	inet_pton(AF_INET, argv[1], &addr_server.sin_addr);

	if(connect(s, (struct sockaddr*) &addr_server, sizeof(addr_server)) == -1)
		report("connect");
	fprintf(stdout, "successfully connected\n");
	
	int sizesend;
	if((sizesend = send(s, argv[3], strlen(argv[3]), 0)) == -1)
		report("send");

	if (sizesend != strlen(argv[3])) fprintf(stdout,"message not fully sent\n");

	fprintf(stdout, "message sent\n");
	
	fprintf(stdout, "done\n");

	exit(EXIT_SUCCESS);
}
