CC=gcc
BUILDDIR:=build
TESTDIR:=computer_enhance/perfaware
TEST2DIR:=test

sim: sim8086.c
	$(CC) -g $^ -o $(BUILDDIR)/$@.out

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

listing039_t: sim
	nasm $(TESTDIR)/part1/listing_0039_more_movs.asm -o $(BUILDDIR)/$@
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	cp $(TESTDIR)/part1/listing_0039_more_movs.asm $(BUILDDIR)/$@.asm
	sed -i '/;/d' $(BUILDDIR)/$@.asm
	diff $(BUILDDIR)/$@.asm $(BUILDDIR)/$@_ds.asm -w -y --color=auto
	
listing040_t: sim
	nasm $(TESTDIR)/part1/listing_0040_challenge_movs.asm -o $(BUILDDIR)/$@
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	cp $(TESTDIR)/part1/listing_0040_challenge_movs.asm $(BUILDDIR)/$@.asm
	sed -i '/;/d' $(BUILDDIR)/$@.asm
	diff $(BUILDDIR)/$@.asm $(BUILDDIR)/$@_ds.asm -w -y --color=auto

listing043_t: sim 
	nasm $(TESTDIR)/part1/listing_0043_immediate_movs.asm -o $(BUILDDIR)/$@
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	cp $(TESTDIR)/part1/listing_0043_immediate_movs.asm $(BUILDDIR)/$@.asm
	sed -i '/;/d' $(BUILDDIR)/$@.asm
	diff $(BUILDDIR)/$@.asm $(BUILDDIR)/$@_ds.asm -w -y --color=auto

listing044_t: sim 
	nasm $(TESTDIR)/part1/listing_0044_register_movs.asm -o $(BUILDDIR)/$@
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	cp $(TESTDIR)/part1/listing_0044_register_movs.asm $(BUILDDIR)/$@.asm
	sed -i '/;/d' $(BUILDDIR)/$@.asm
	diff $(BUILDDIR)/$@.asm $(BUILDDIR)/$@_ds.asm -w -y --color=auto

listing046_t: sim
	nasm $(TESTDIR)/part1/listing_0046_add_sub_cmp.asm -o $(BUILDDIR)/$@
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	cp $(TESTDIR)/part1/listing_0046_add_sub_cmp.asm $(BUILDDIR)/$@.asm
	sed -i '/;/d' $(BUILDDIR)/$@.asm
	diff $(BUILDDIR)/$@.asm $(BUILDDIR)/$@_ds.asm -w -y --color=auto

listing048_t: sim
	nasm $(TESTDIR)/part1/listing_0048_ip_register.asm -o $(BUILDDIR)/$@
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	cp $(TESTDIR)/part1/listing_0048_ip_register.asm $(BUILDDIR)/$@.asm
	sed -i '/;/d' $(BUILDDIR)/$@.asm
	diff $(BUILDDIR)/$@.asm $(BUILDDIR)/$@_ds.asm -w -y --color=auto

listing049_t: sim
	nasm $(TESTDIR)/part1/listing_0049_conditional_jumps.asm -o $(BUILDDIR)/$@
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	cp $(TESTDIR)/part1/listing_0049_conditional_jumps.asm $(BUILDDIR)/$@.asm
	sed -i '/;/d' $(BUILDDIR)/$@.asm
	diff $(BUILDDIR)/$@.asm $(BUILDDIR)/$@_ds.asm -w -y --color=auto

listing051_t: sim
	nasm $(TESTDIR)/part1/listing_0051_memory_mov.asm -o $(BUILDDIR)/$@
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	cp $(TESTDIR)/part1/listing_0051_memory_mov.asm $(BUILDDIR)/$@.asm
	sed -i '/;/d' $(BUILDDIR)/$@.asm
	diff $(BUILDDIR)/$@.asm $(BUILDDIR)/$@_ds.asm -w -y --color=auto

listing056_t: sim
	nasm $(TESTDIR)/part1/listing_0056_estimating_cycles.asm -o $(BUILDDIR)/$@
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	$(BUILDDIR)/$^.out $(BUILDDIR)/$@ > $(BUILDDIR)/$@_ds.asm
	cp $(TESTDIR)/part1/listing_0056_estimating_cycles.asm $(BUILDDIR)/$@.asm
	sed -i '/;/d' $(BUILDDIR)/$@.asm
	diff $(BUILDDIR)/$@.asm $(BUILDDIR)/$@_ds.asm -w -y --color=auto


test_mem: sim
	nasm $(TEST2DIR)/mem_to_acc.asm -o $(BUILDDIR)/mem_to_acc
	$(BUILDDIR)/$^.out $(BUILDDIR)/mem_to_acc > $(BUILDDIR)/$@_ds.asm

test_arthmetic: sim
	nasm $(TEST2DIR)/add_reg.asm -o $(BUILDDIR)/add_reg
	nasm $(TEST2DIR)/sub_reg.asm -o $(BUILDDIR)/sub_reg
	$(BUILDDIR)/$^.out $(BUILDDIR)/add_reg > $(BUILDDIR)/$@_ds.asm
#	echo "---" >> $@_ds.asm
#	$(BUILDDIR)/$^.out $(BUILDDIR)/sub_reg >> $(BUILDDIR)/$@_ds.asm

test_cmp_reg: sim
	nasm $(TEST2DIR)/cmp.asm -o $(BUILDDIR)/cmp

test_jmp_reg: sim
	nasm $(TEST2DIR)/jmp.asm -o $(BUILDDIR)/jmp

clean: 
	rm -r build/*
