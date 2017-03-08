CC=g++
CFLAGS= -std=c++11 -Wall -Wextra
default: pasians-cli

paslib.o: paslib.cc paslib.h
	$(CC) $(CFLAGS) paslib.cc -c -o paslib.o

pasians-cli: paslib.o pasians-cli.cc pasians-cli.h
	$(CC) $(CFLAGS) pasians-cli.cc paslib.o -o pasians-cli

clean:
	rm -f paslib.o pasians-cli
