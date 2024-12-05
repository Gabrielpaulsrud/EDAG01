CC	= cc
# put the one you prefer last
CFLAGS	= -g -O3 -std=c18 -pedantic -Werror -mcpu=power8 -pthread
CFLAGS	= -g -O3 -std=c18 -pedantic -Werror
# CFLAGS	= -g

LDFLAGS	= -g -pthread -lm

OBJS	= intopt.o main.o error.o

main: $(OBJS)	
	$(CC) $(OBJS) $(CFLAGS) $(LDFLAGS)
	rm -f score
	./a.out -t 5

# Intent target with intopt.c
intopt: intopt.o
	$(CC) $(CFLAGS) intopt.o $(LDFLAGS) -o intopt.out
	rm -f score

# Debug target (no optimization, with debugging symbols)
debug: CFLAGS = -g -std=c18 -pedantic -Werror
debug: $(OBJS) main.o
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o a.out


# gprof target (no optimization, with debugging symbols)
#gprof: CFLAGS = -pg -g -std=c18 -pedantic -Werror
#gprof: $(OBJS) main.o
#  $(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o a.out
#  gprof -T a.out

# gprof target (no optimization, with debugging symbols)
gprof: CFLAGS = -pg -g -std=c18 -pedantic -Werror
gprof: $(OBJS) main.o
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o a.out
	# gprof -T a.out


# gcov target
gcov: CFLAGS = -fprofile-arcs -ftest-coverage -g -std=c18 -pedantic -Werror
gcov: $(OBJS) main.o
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o a.out
	#gcov -T a.out

fsan: CFLAGS = -g -fsanitize=address -fsanitize=undefined -std=c18 -pedantic -Werror
fsan: $(OBJS) main.o
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o a.out	

clean:
	rm -f *.o a.out *.gcno *.gcda

# for use on power.cs.lth.se and other linux machines with oprofile
operf: a.out
	operf -e CYCLES:100000:0:0:1 ./a.out -t 120
