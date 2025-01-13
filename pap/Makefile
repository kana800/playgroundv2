CC=gcc
BUILDDIR:=build
TESTDIR:=computer_enhance/perfaware

sim: sim8086.c
	$(CC) -g $^ -o $(BUILDDIR)/$@.out


day1: sim
	nasm $(TESTDIR)/part1/listing_0037_single_register_mov.asm -o $(BUILDDIR)/$@
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@

day1test: sim
	nasm $(TESTDIR)/part1/listing_0037_single_register_mov.asm -o $(BUILDDIR)/$@
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	cp $(TESTDIR)/part1/listing_0037_single_register_mov.asm $(BUILDDIR)/$@.asm
	sed -i '/;/d' $(BUILDDIR)/$@.asm
	diff -w -i -Bb -qsy --color=auto $(BUILDDIR)/$@.asm $(BUILDDIR)/$@_ds.asm

clean: 
	rm -r build/*
