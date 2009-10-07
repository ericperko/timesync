%.out: %.c
	gcc -o "$@" "$<"

all: server.out receiver.out
