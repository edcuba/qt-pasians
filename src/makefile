CC=g++
CFLAGS = -std=c++11 -Wall -Wextra
default: pasians-cli

json.o: jsoncpp.cpp
	$(CC) $(CFLAGS) $^ -c -o $@

paslib.o: paslib.cc
	$(CC) $(CFLAGS) $^ -c -o $@

pasians-cli: paslib.o json.o pasians-cli.cc
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f paslib.o pasians-cli
