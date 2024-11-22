CC=gcc
CFLAGS = -mavx

avx: 
	$(CC) $(CFLAGS) avx.c -o $@.o

.PHONY: clean
clean:
	rm -rf *.o
