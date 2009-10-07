#include "common.h"

struct timeval timerdiv(const struct timeval *tv, int n);

int main(int argc, char *argv[]) {
	char *hostname;
	if (argc == 2) {
		hostname = argv[1];
	}
	else {
		printf("usage: receiver server_ip\n");
		exit(0);
	}
	struct sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_port = htons(receiver_port);
	client.sin_addr.s_addr = htonl(INADDR_ANY);
	socklen_t serverlen = sizeof(struct sockaddr_in);
	int socket_c = socket(PF_INET, SOCK_DGRAM, 0);
	if(bind(socket_c, (struct sockaddr *) &client, serverlen) < 0) {
		perror("Error binding socket");
		exit(-1);
	}	
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(server_port);
	inet_aton(hostname, &(server.sin_addr));
	struct timeval t1;
	gettimeofday(&t1, NULL);
	Time t;
	bzero(&t, sizeof(Time));
	t.current_time = t1; //Doesn't really have to be a time to send to the other machine... but why not?
	int bytes = sendto(socket_c, (char *) &t, sizeof(Time), 0, (struct sockaddr *) &server, serverlen);
	printf("sent bytes to server: %d\n", bytes);
	if(bytes < 0) {
		perror("Error sending message to the server");
		exit(-1);
	}
	bytes = recvfrom(socket_c, (char *) &t, sizeof(Time), 0, (struct sockaddr *) &server, &serverlen);
	if (bytes < 0) {
		perror("Error receiving message from the server");
		exit(-1);
	} else if (bytes == sizeof(Time)){ // we got a whole struct, so lets do something with it
		struct timeval t4;
		gettimeofday(&t4, NULL);
		struct timeval dTreq, dTres, delayx2;
		timersub(&(t.piggyback_time), &t1, &dTreq);
		timersub(&(t.current_time), &t4, &dTres);
		timeradd(&dTreq, &dTres, &delayx2);
		struct timeval delay = timerdiv(&delayx2, 2);
		printf("Clock difference is %ds:%dus\n", (int) delay.tv_sec, (int) delay.tv_usec);
	} else {
		perror("Didn't receive a whole Time struct");
		exit(-1);
	}
	return 0;
}

struct timeval timerdiv(const struct timeval *tv, int n) {
	struct timeval v;
	v.tv_sec = tv->tv_sec / n;
        v.tv_usec = ((tv->tv_sec % n) * ONE_MILLION + tv->tv_usec) / n;
	return v;
}	
