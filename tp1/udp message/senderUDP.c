#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

void report(char* src) {
	fprintf(stderr, "%s : %s\n", src, strerror(errno)); 
	exit(EXIT_FAILURE);
}

int main (int argc, char* argv[]) {

	if (argc != 4) {
		fprintf(stderr, "usage: ./senderUDP <adresse_IP_dest> <num_port_distant> <message>\n");
		exit(EXIT_FAILURE);
	} 
	
	int s;
	//intialise socket
	s = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (s == -1)
		report("socket");
	
	struct sockaddr_in addr_server;
	addr_server.sin_family = AF_INET;
	addr_server.sin_port = htons(atoi(argv[2]));
	
	inet_pton(AF_INET, argv[1], &addr_server.sin_addr);
		
	if (sendto(s, argv[3], strlen(argv[3]), 0, (struct sockaddr*) &addr_server, sizeof(addr_server)) == -1)
		report("sendto"); 
		
	close(s);
	exit (EXIT_SUCCESS);
}
