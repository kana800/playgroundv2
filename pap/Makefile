CC=gcc
BUILDDIR:=build
TESTDIR:=computer_enhance/perfaware
TEST2DIR:=test

hav_gen: hav_gen.c hav_formula.h pcg64.h
	$(CC) -g $^ -lm -o $(BUILDDIR)/$@.out

parser: jsonparser.c
	$(CC) -g $^ -o $(BUILDDIR)/$@.out

clean: 
	rm -r build/*
