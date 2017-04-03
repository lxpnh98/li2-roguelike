CFLAGS=-Wall -g
FICHEIROS=cgi.h estado.c estado.h exemplo.c Makefile

install: exemplo
	sudo cp exemplo /usr/lib/cgi-bin
	sudo cp ../images/* /var/www/html/images/
	touch install

exemplo: exemplo.o estado.o
	cc -o exemplo exemplo.o estado.o

zip: $(FICHEIROS)
	zip -9 exemplo.zip $(FICHEIROS)

doc:
	doxygen -g
	doxygen
	mv Doxyfile html/ latex/ ../doc/
	touch doc

clean:
	rm -rf *.o exemplo ../doc/Doxyfile ../doc/latex ../doc/html install doc

estado.o: estado.c estado.h
exemplo.o: exemplo.c cgi.h estado.h

run-firefox:
	firefox "localhost/cgi-bin/exemplo"
