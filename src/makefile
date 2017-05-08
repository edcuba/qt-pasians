CC=g++
CFLAGS = -std=c++11 -Wall -Wextra -O2
PROFILE = pasians-gui/pasians-gui.pro
TARGETCLI = ../hra2017-cli
TARGET = ../hra2017
default: $(TARGETCLI) $(TARGET)

json.o: jsoncpp.cpp
	$(CC) $(CFLAGS) $^ -c -o $@

paslib.o: paslib.cc
	$(CC) $(CFLAGS) $^ -c -o $@

$(TARGETCLI): paslib.o json.o pasians-cli.cc
	$(CC) $(CFLAGS) $^ -o $@

$(TARGET): paslib.o
	mkdir -p build
	cd build && qmake ../$(PROFILE)
	cd build && make

clean:
	rm -f *.o
	rm -rf build
