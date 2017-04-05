CFLAGS=-Wall -g
FICHEIROS=cgi.h estado.c estado.h html.c html.h main.c Makefile

install: main
	sudo cp main /usr/lib/cgi-bin
	sudo cp ../images/* /var/www/html/images/
	touch install

main: main.o estado.o html.o
	cc -o main main.o estado.o html.o

zip: $(FICHEIROS)
	zip -9 main.zip $(FICHEIROS)

doc:
	doxygen -g
	doxygen
	mv Doxyfile html/ latex/ ../doc/
	touch doc

clean:
	rm -rf *.o main ../doc/Doxyfile ../doc/latex ../doc/html install doc

estado.o: estado.c estado.h cgi.h
html.o: html.c html.h cgi.h
main.o: main.c cgi.h

run-firefox:
	firefox "localhost/cgi-bin/main"
