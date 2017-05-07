all: doxygen pasians

pasians:
	cd src && make

doxygen:
	cd doc && doxygen Doxyfile

clean:
	cd src && make clean
	rm -rf doc/html
