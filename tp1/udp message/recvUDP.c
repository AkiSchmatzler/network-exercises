/**
* \author Aki Schmatzler
* \brief server for a very simple udp message sender
* \date 10/10/21
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024

void report(char* src) {
	fprintf(stderr, "%s : %s\n", src, strerror(errno)); 
	exit(EXIT_FAILURE);
}


int main (int argc, char* argv[]) {

	if (argc != 2) {
		fprintf(stderr, "usage: ./recvUDP <num_port_local>\n");
		exit(EXIT_FAILURE);
	} 

	//s = socket descriptor
	int s;
	char buf_msg[BUF_SIZE];
	char buf_addr[BUF_SIZE];
	memset(buf_addr, 0, BUF_SIZE);
	memset(buf_addr, 0, BUF_SIZE);
	
	//creating socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) report("socket");
	
	//structure for adresses
	struct sockaddr_in client_addr;
	struct sockaddr_in my_addr = {
		AF_INET,
		htons(atoi(argv[1])),
		htonl(INADDR_ANY)
	};
		
	//binding our address to the socket
	if (bind(s, (struct sockaddr*) &my_addr, sizeof(my_addr)) == -1) 
		report("bind");
	
	
	//prepare reception in buffer
	int len;
	int n;
	if ((n = recvfrom (s, &buf_msg, BUF_SIZE, 0, (struct sockaddr*) &client_addr, &len)) == -1) 
		report("recvfrom");
	buf_msg[n] = '\0';
	
	
	//getting address of sender
	if(inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, buf_addr, BUF_SIZE) <=0) 
		report("inet_ntop");
	
	
	//print message received and source
	fprintf(stdout, "message received from : %s\n\
			number of received characters : %d\n\
			sent on port : %d\n\
			received message : %s\n", buf_addr, n, ntohs(client_addr.sin_port), buf_msg);
			
	
	
	fprintf(stdout, "\n#   exiting and closing socket - thank u   #\n");
	
	if (close(s) == -1) report("close");
	
	exit(EXIT_SUCCESS);
}

