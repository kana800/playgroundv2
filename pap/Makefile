CC=gcc
BUILDDIR:=build
TESTDIR:=computer_enhance/perfaware
TEST2DIR:=test

sim: sim8086.c
	$(CC) -g $^ -o $(BUILDDIR)/$@.out

l037: sim
	nasm $(TESTDIR)/part1/listing_0037_single_register_mov.asm -o $(BUILDDIR)/$@

l038: sim
	nasm $(TESTDIR)/part1/listing_0038_many_register_mov.asm -o $(BUILDDIR)/$@

listing037_t: sim
	nasm $(TESTDIR)/part1/listing_0037_single_register_mov.asm -o $(BUILDDIR)/$@
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	cp $(TESTDIR)/part1/listing_0037_single_register_mov.asm $(BUILDDIR)/$@.asm
	sed -i '/;/d' $(BUILDDIR)/$@.asm
	diff $(BUILDDIR)/$@.asm $(BUILDDIR)/$@_ds.asm -w -y --color=auto

listing038_t: sim
	nasm $(TESTDIR)/part1/listing_0038_many_register_mov.asm -o $(BUILDDIR)/$@
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	cp $(TESTDIR)/part1/listing_0038_many_register_mov.asm $(BUILDDIR)/$@.asm
	sed -i '/;/d' $(BUILDDIR)/$@.asm
	diff $(BUILDDIR)/$@.asm $(BUILDDIR)/$@_ds.asm -w -y --color=auto

test: sim
	nasm $(TEST2DIR)/immediate_to_register.asm -o $(BUILDDIR)/imm_to_test
	$(BUILDDIR)/$^.out $(BUILDDIR)/imm_to_test

clean: 
	rm -r build/*
