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

test_imm_to_reg: sim
	nasm $(TEST2DIR)/immediate_to_register.asm -o $(BUILDDIR)/imm_to_test
	$(BUILDDIR)/$^.out $(BUILDDIR)/imm_to_test > $(BUILDDIR)/$@_ds.asm
	cp $(TEST2DIR)/immediate_to_register.asm $(BUILDDIR)/
	diff $(BUILDDIR)/immediate_to_register.asm $(BUILDDIR)/$@_ds.asm -w -y --color=auto

test_bit_disp: sim
	nasm $(TEST2DIR)/bit_displacement.asm -o $(BUILDDIR)/bit_disp
	$(BUILDDIR)/$^.out $(BUILDDIR)/bit_disp > $(BUILDDIR)/$@_ds.asm
	cp $(TEST2DIR)/bit_displacement.asm $(BUILDDIR)/
	diff $(BUILDDIR)/bit_displacement.asm $(BUILDDIR)/$@_ds.asm -w -y --color=auto

test_neg_bit_disp: sim
	nasm $(TEST2DIR)/neg_bit_disp.asm -o $(BUILDDIR)/neg_bit_disp
	$(BUILDDIR)/$^.out $(BUILDDIR)/neg_bit_disp > $(BUILDDIR)/$@_ds.asm
	cp $(TEST2DIR)/neg_bit_displacement.asm $(BUILDDIR)/
	diff $(BUILDDIR)/neg_bit_displacement.asm $(BUILDDIR)/$@_ds.asm -w -y --color=auto
test_exp_size: sim

	nasm $(TEST2DIR)/explicit_sizes.asm -o $(BUILDDIR)/exp_size
	$(BUILDDIR)/$^.out $(BUILDDIR)/exp_size > $(BUILDDIR)/$@_ds.asm

test_direct_addr: sim
	nasm $(TEST2DIR)/direct_address.asm -o $(BUILDDIR)/direct_addr
	$(BUILDDIR)/$^.out $(BUILDDIR)/direct_addr > $(BUILDDIR)/$@_ds.asm

test_mem_to_acc: sim
	nasm $(TEST2DIR)/mem_to_acc.asm -o $(BUILDDIR)/mem_to_acc
	$(BUILDDIR)/$^.out $(BUILDDIR)/mem_to_acc > $(BUILDDIR)/$@_ds.asm

test_add_reg: sim
	nasm $(TEST2DIR)/add_reg.asm -o $(BUILDDIR)/add_reg

test_sub_reg: sim
	nasm $(TEST2DIR)/sub_reg.asm -o $(BUILDDIR)/sub_reg

test_cmp_reg: sim
	nasm $(TEST2DIR)/cmp.asm -o $(BUILDDIR)/cmp

test_jmp_reg: sim
	nasm $(TEST2DIR)/jmp.asm -o $(BUILDDIR)/jmp

clean: 
	rm -r build/*
