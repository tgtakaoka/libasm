VPATH=../src
CXXFLAGS=-Wall -g -std=c++17 -O -I$(VPATH)
TESTS= \
	test_asm_mc6809 test_asm_hd6309 \
	test_dis_mc6809 test_dis_hd6309 \
	test_asm_r6502  test_dis_r6502 \
	test_asm_r65c02 test_dis_r65c02 \
	test_asm_i8080  test_dis_i8080 \
	test_dis_tms9995 \
	test_asm_z80    test_dis_z80
COM_OBJS = string_utils.o test_asserter.o
OBJS_mc6809 = registers_hd6309.o table_mc6809.o
OBJS_i8080 = registers_i8080.o table_i8080.o
OBJS_z80 = registers_z80.o table_z80.o
OBJS= $(COM_OBJS) \
	$(OBJS_mc6809) table_hd6309.o \
	table_r6502.o table_r65c02.o \
	asm_i8080.o  dis_i8080.o  $(OBJS_i8080) \
	dis_tms9995.o table_tms9995.o \
	asm_z80.o    dis_z80.o    $(OBJS_Z80) \

.PHONY: clean clean-obj test \
	test_mc6809 test_hd6309 \
	test_r6502 test_r65c02 \
	test_i8080 test_z80 \
	test_tms9995

test: test_mc6809 test_hd6309 \
	test_r6502 test_r65c02 \
	test_i8080 test_z80 \
	test_tms9995

test_mc6809: test_asm_mc6809 test_dis_mc6809
	./test_asm_mc6809
	./test_dis_mc6809
test_hd6309: test_asm_hd6309 test_dis_hd6309
	./test_asm_hd6309
	./test_dis_hd6309
test_r6502: test_asm_r6502 test_dis_r6502
	./test_asm_r6502
	./test_dis_r6502
test_r65c02: test_asm_r65c02 test_dis_r65c02
	./test_asm_r65c02
	./test_dis_r65c02
test_i8080:  test_asm_i8080  test_dis_i8080
	./test_asm_i8080
	./test_dis_i8080
test_tms9995:test_dis_tms9995
	./test_dis_tms9995
test_z80: test_asm_z80 test_dis_z80
	./test_asm_z80
	./test_dis_z80


test_asm_mc6809: test_asm_mc6809.o $(OBJS_mc6809) $(COM_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_hd6309: test_asm_hd6309.o $(OBJS_mc6809) table_hd6309.o $(COM_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_r6502:  test_asm_r6502.o table_r6502.o $(COM_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_r65c02: test_asm_r65c02.o table_r6502.o table_r65c02.o $(COM_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_i8080:  test_asm_i8080.o asm_i8080.o $(OBJS_i8080) $(COM_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_z80:    test_asm_z80.o asm_z80.o $(OBJS_z80) $(COM_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_mc6809: test_dis_mc6809.o $(OBJS_mc6809) $(COM_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_hd6309: test_dis_hd6309.o $(OBJS_mc6809) table_hd6309.o $(COM_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_r6502:  test_dis_r6502.o  table_r6502.o $(COM_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_r65c02: test_dis_r65c02.o table_r6502.o table_r65c02.o $(COM_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_i8080:  test_dis_i8080.o dis_i8080.o $(OBJS_i8080) $(COM_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_tms9995:test_dis_tms9995.o dis_tms9995.o table_tms9995.o $(COM_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_z80  :  test_dis_z80.o dis_z80.o $(OBJS_z80) $(COM_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^

test_asm_mc6809.o: asm_hd6309.h test_asm_helper.h test_asserter.h test_symtab.h
test_asm_hd6309.o: asm_hd6309.h test_asm_helper.h test_asserter.h test_symtab.h
test_asm_r6502.o:  asm_r65c02.h test_asm_helper.h test_asserter.h test_symtab.h
test_asm_r65c02.o: asm_r65c02.h test_asm_helper.h test_asserter.h test_symtab.h
test_asm_i8080.o:  asm_i8080.h  test_asm_helper.h test_asserter.h test_symtab.h
test_asm_z80.o:    asm_z80.h    test_asm_helper.h test_asserter.h test_symtab.h
test_dis_mc6809.o: dis_hd6309.h test_dis_helper.h test_asserter.h test_memory.h test_symtab.h
test_dis_hd6309.o: dis_hd6309.h test_dis_helper.h test_asserter.h test_memory.h test_symtab.h
test_dis_r6502.o:  dis_r65c02.h test_dis_helper.h test_asserter.h test_memory.h test_symtab.h
test_dis_r65c02.o: dis_r65c02.h test_dis_helper.h test_asserter.h test_memory.h test_symtab.h
test_dis_i8080.o:  dis_i8080.h  test_dis_helper.h test_asserter.h test_memory.h test_symtab.h
test_dis_tms9995.o:dis_tms9995.h test_dis_helper.h test_asserter.h test_memory.h test_symtab.h
test_dis_z80.o:    dis_z80.h    test_dis_helper.h test_asserter.h test_memory.h test_symtab.h

test_asserter.o: test_asserter.h
test_memory.h: memory.h
test_symtab.h: symbol_table.h

clean: clean-obj
	-rm -f $(TESTS)
clean-obj:
	-rm -f *.o
