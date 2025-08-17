CC=g++
FLAGS=-g

asm: listing_0132_nop_loop.asm
	nasm -f elf64 -o listing_0132_nop_loop.obj $^
	ar rcs listing_0132_nop_loop.a listing_0132_nop_loop.obj

front_end_test: listing_0133_front_end_test_main.cpp
	$(CC) $^ -g -llisting_0132_nop_loop -o $@.out

clean:
	rm -r *.a *.out *.obj
