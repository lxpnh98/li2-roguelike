CFLAGS=-Wall -g -ansi -Wextra -pedantic -O2 -lm
FICHEIROS=../src/ ../images/ ../doc/

install: main
	sudo cp main /usr/lib/cgi-bin
	sudo cp -r ../images/ /var/www/html/
	touch /var/www/estado
	chmod 777 /var/www/estado
	touch install

main: main.o estado.o html.o aux.o
	cc -o main main.o estado.o html.o aux.o

zip: $(FICHEIROS)
	zip -r -9 main.zip $(FICHEIROS)

doc:
	doxygen -g
	doxygen
	mv Doxyfile html/ latex/ ../doc/
	touch doc

clean:
	rm -rf *.o main ../doc/Doxyfile ../doc/latex ../doc/html install doc /var/www/estado

estado.o: estado.c estado.h aux.h cgi.h
html.o: html.c html.h cgi.h
main.o: main.c cgi.h

run-firefox:
	firefox "localhost/cgi-bin/main" &>/dev/null &
