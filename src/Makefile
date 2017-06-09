CFLAGS=-Wall -g -Wextra -pedantic -O2 -lm
FICHEIROS=../src/ ../images/ ../doc/

install: main
	sudo cp main /usr/lib/cgi-bin
	sudo cp -r ../images/ /var/www/html/
	sudo touch /var/www/estado
	sudo chmod 777 /var/www/estado
	sudo touch /var/www/scores
	sudo chmod 777 /var/www/scores
	sudo touch install

main: main.o estado.o html.o calc.o ficheiro.o
	cc -o main main.o estado.o html.o calc.o ficheiro.o

zip: $(FICHEIROS)
	zip -r -9 main.zip $(FICHEIROS)

doc:
	doxygen -g
	doxygen
	mv Doxyfile html/ latex/ ../doc/
	touch doc

clean:
	sudo rm -rf *.o main ../doc/Doxyfile ../doc/latex ../doc/html install doc /var/www/estado /var/www/scores

estado.o: estado.c estado.h calc.h cgi.h
html.o: html.c html.h cgi.h ficheiro.h
main.o: main.c cgi.h ficheiro.h

run-firefox:
	firefox "localhost/cgi-bin/main" &>/dev/null &
