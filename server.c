#include "common.h"

int main(int argc, char *argv[]) {
	int socket_s = socket(PF_INET, SOCK_DGRAM, 0);
	if (socket_s < 0) {
		perror("Error creating socket");
		exit(-1);	
	}
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(server_port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	socklen_t serverlen = sizeof(server_addr);	
	if (bind(socket_s, (struct sockaddr *) &server_addr, serverlen) < 0) {
		perror("Error binding socket");
		exit(-1);
	}
	printf("Server running on port %d... waiting for connections\n",
			server_port);
	while(1) {
		struct sockaddr_in client_addr;
		char buffer[1024];
		int bytes = 0;
		bytes = recvfrom(socket_s, &buffer, 1024, 0, (struct sockaddr *)
			&client_addr, &serverlen);
		if(bytes < 0) {
			perror("Error in the recvfrom");
		} else if (bytes != sizeof(Time)) {
			perror("Received something that is not the size of Time");
		} else {
			//We recieved the correct number of bytes for a Time
			//packet
			struct timeval t2;
			gettimeofday(&t2, NULL);	//time when we received it
			Time *times = (Time *) &buffer[0];	
			times->piggyback_time = t2;
			struct timeval t3;
			gettimeofday(&t3, NULL); 	//time when we sent it
			times->current_time = t3;
			int bytes = sendto(socket_s, (char *) times, sizeof(Time), 0, (struct sockaddr *) &client_addr, serverlen);
			if (bytes < 0) {
				perror("Error sending reply to client");
			}
		}
	}
	return 0;
}
