default: pasians

pasians:
	cd src && make

doxygen:
	cd doc && doxygen Doxyfile

clean:
	cd src && make clean
	rm -rf doc/html
	rm -f hra2017 hra2017-cli
	rm -f xcubae00-xkurak00.tar.gz

run:
	./hra2017 &
	./hra2017-cli

pack: clean
	tar -zcf xcubae00-xkurak00.tar.gz src examples Makefile doc README.txt
