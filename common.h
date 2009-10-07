#define server_port 60000
#define receiver_port 60001

#define ONE_MILLION 1000000

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

typedef struct time_message {
	struct timeval piggyback_time;
	struct timeval current_time;
} Time;
