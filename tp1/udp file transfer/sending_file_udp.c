/**
* \author Aki Schmatzler
* \brief client for a very simple udp file transfer
* \date 10/10/21
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_READ 1024


void report(char* src) {
	fprintf(stderr, "%s : %s\n", src, strerror(errno)); 
	exit(EXIT_FAILURE);
}


int main(int argc, char* argv[]) {

	if (argc != 4) {
		fprintf(stderr, "usage : ./sending_file_udp <remote address> <remote port> <file to send>\n");
		exit(EXIT_FAILURE);
	}

	//buffer for reading in the file
	char buf_read[MAX_READ];
	memset(buf_read, 0, MAX_READ);
	int sizeread = 1;


	//opening the file
	int fd;
	if ((fd = open(argv[3], O_RDONLY)) == -1)
		report("open");

	fprintf(stdout, "file opened\n");

	//intialise socket
	int s;
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		report("socket");

	fprintf(stdout, "socket created\n");

	//filling structure with server info
	struct sockaddr_in addr_server;
	addr_server.sin_family = AF_INET;
	addr_server.sin_port = htons(atoi(argv[2]));
	
	inet_pton(AF_INET, argv[1], &addr_server.sin_addr);

	int totalsize;

	while (sizeread != 0) {
		memset(buf_read, 0, MAX_READ);
		if ((sizeread = read(fd, buf_read, MAX_READ)) == -1)
			report ("read");

		if (sendto(s, buf_read, strlen(buf_read), 0, (struct sockaddr*) &addr_server, sizeof(addr_server)) == -1)
			report("sendto");

		totalsize += sizeread;
	}

	fprintf(stdout, "%d characters sent, exiting\n", totalsize);
	
	if (close(s) == -1)
		report("close socket");
	if (close(fd) == -1)
		report("close file descriptor");

	exit(EXIT_SUCCESS);
}