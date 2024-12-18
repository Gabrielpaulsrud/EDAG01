CC	= cc
CFLAGS	= -std=c11 -pedantic -Wall -Werror -g -O3 -funsigned-char
OUT	= a.out
FILE	= poly

execute: compile
	./a.out > output
	diff output correct

compile: clean $(FILE).o malloc.o main.o
	$(CC) $(CFLAGS) -o $(OUT) $(FILE).o malloc.o main.o

$(FILE).o: $(FILE).c
	$(CC) -include wecc.h -c $(FILE).c $(CFLAGS)

malloc.o: malloc.c
	$(CC) -c malloc.c $(CFLAGS) 

clean:
	rm -f *.s *.o $(OUT) output

test: clean $(FILE).o malloc.o main.o
	gcc -g -O0 -fsanitize=address -o $(OUT) $(FILE).o malloc.o main.o 
	./a.out

mem: clean $(FILE).o malloc.o main.o
	gcc -g -O0 -o $(OUT) $(FILE).o malloc.o main.o

