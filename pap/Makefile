CC=gcc

BUILDDIR := build
TESTDIR := test

sim: sim8086.c
	$(CC) $^ -g -o $(BUILDDIR)/a.out

day1test: sim
	nasm $(TESTDIR)/listing_0037_single_register_mov.asm -o $(BUILDDIR)/day1test1
	nasm $(TESTDIR)/listing_0038_many_register_mov.asm -o $(BUILDDIR)/day1test2
	$(BUILDDIR)/a.out $(BUILDDIR)/day1test1
	echo "T2"
	$(BUILDDIR)/a.out $(BUILDDIR)/day1test2
