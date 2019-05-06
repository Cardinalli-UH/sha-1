
final: final.o printbits.o
	gcc final.o printbits.o -o final -lm

final.o: final.c printbits.h
	gcc -c final.c

printbits.o: printbits.c printbits.h
	gcc -c printbits.c