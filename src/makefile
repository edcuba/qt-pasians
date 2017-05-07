CC=g++
CFLAGS = -std=c++11 -Wall -Wextra -O0 -g
default: hra2017-cli

json.o: jsoncpp.cpp
	$(CC) $(CFLAGS) $^ -c -o $@

paslib.o: paslib.cc
	$(CC) $(CFLAGS) $^ -c -o $@

hra2017-cli: paslib.o json.o pasians-cli.cc
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f paslib.o hra2017-cli
