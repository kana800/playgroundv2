CC=gcc

BUILDDIR:=build
GMP:=external/gmp-6.3.0/gmp.h

clean: 
	rm -rf *.o *.out 
	echo clean completed

day1: 1.c
	$(CC) -g $^ -o $@.o

day2: 2.c
	$(CC) -g $^ -o $@.o

day3: 3.c
	$(CC) -g $^ -o $@.o

day4: 4.c
	$(CC) -g $^ -o $@.o

day5: 5.c
	$(CC) -g $^ -o $@.o

day6: 6.c
	$(CC) -g $^ -o $@.o

day7: 7.c
	$(CC) -g $^ -o $@.o

day8: 8.c
	$(CC) -g $^ -o $@.o

day9: 9.c
	$(CC) -g $^ -o $@.o

day10: 10.c primes.h
	$(CC) -g $^ -o $@.o

day11: 11.c
	$(CC) -g $^ -o $@.o


day12: 12.c
	$(CC) -g -lm $^ -o $@.o

day16: 16.c
	$(CC) -g -lm $^ -o $@.o

48: 48.c
#	cp -r external/gmp-6.3.0/libgmp.la $(BUILDDIR)/
	$(CC) -g $^ $(GMP) -L$(BUILDDIR)/ -l:libgmp.a -o $(BUILDDIR)/$@.o
44: 44.c
#	cp -r external/gmp-6.3.0/libgmp.la $(BUILDDIR)/
#	$(CC) -g $^ $(GMP) -L$(BUILDDIR)/ -l:libgmp.a -o $(BUILDDIR)/$@.o
	$(CC) -g $^ -lm -o $@.o
