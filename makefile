legendre: src/main.o src/cJSON.o
  gcc -o legendre src/main.o src/cJSON.o -lm

src/main.o: src/main.c src/legendre.h src/cJSON.h makefile
	gcc -c src/main.c -o src/main.o

src/cJSON.o: src/cJSON.c src/cJSON.h makefile
	gcc -c src/cJSON.c -o src/cJSON.o

clean:
	rm src/cJSON.o src/main.o
