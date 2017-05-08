default: pasians

pasians:
	cd src && make
	cd src/gui && make

doxygen:
	cd doc && doxygen Doxyfile

clean:
	cd src && make clean
	cd src/gui && make clean
	rm -rf doc/html
	rm -f hra2017 hra2017-cli

run:
	./hra2017 &
	./hra2017-cli

pack: clean
	tar -zcf xcubae00-xkurak00.tar.gz src Makefile doc README.txt
