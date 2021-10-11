/**
* \author Aki Schmatzler
* \brief server for a very simple udp file transfer
* \date 10/10/21
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>

#define MAX_WRITE 1024


void report(char* src) {
	fprintf(stderr, "%s : %s\n", src, strerror(errno)); 
	exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {

	if (argc != 3){
		fprintf(stderr, "usage : ./getting_file_udp <port_number> <output_file>\n");
		exit(EXIT_FAILURE);
	}
		

	//opening socket
	int s;
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
		report("socket");
	

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

	//buffered writing in the file
	int fd;
	char buf_read[MAX_WRITE];
	memset(buf_read, 0, MAX_WRITE);
	int sizewrite = 1;
	if ((fd = open(argv[2], O_CREAT|O_TRUNC|O_WRONLY, 0666)) == -1)
		report("open");

	int len;
	int n;
	int totalsizewritten = 0;
	
	while (sizewrite != 0) {
		//prepare reception in buffer
		memset(buf_read, 0, MAX_WRITE);

		//reading in socket and putting data in the buffer
		if ((n = read (s, buf_read, MAX_WRITE)) == -1) 
			report("recvfrom");

		//copying what was read in the buffer in the file
		if ((sizewrite = write(fd, buf_read, n)) == -1)
			report ("read");
		totalsizewritten += sizewrite;
	}

	fprintf(stdout, "received %d characters, exiting\n", totalsizewritten);

	if (close(fd) == -1) report("close file descriptor");
	if (close(s) == -1) report("close socket");

	exit(EXIT_SUCCESS);
}