VPATH=../src
TESTS= \
	test_asm_mc6809 test_asm_hd6309 \
	test_dis_mc6809 test_dis_hd6309 \
	test_asm_r6502  test_dis_r6502 \
	test_asm_r65c02 test_dis_r65c02 \
	test_asm_i8080  test_dis_i8080 \
	test_asm_tms9995 test_dis_tms9995 \
	test_asm_z80    test_dis_z80

.PHONY: clean clean-obj test \
	test_mc6809 test_hd6309 \
	test_r6502 test_r65c02 \
	test_i8080 test_z80 \
	test_tms9995

test:   test_mc6809 test_hd6309 \
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
test_tms9995:test_asm_tms9995 test_dis_tms9995
	./test_asm_tms9995
	./test_dis_tms9995
test_z80: test_asm_z80 test_dis_z80
	./test_asm_z80
	./test_dis_z80

CXXFLAGS    =-Wall -g -std=c++17 -O -I$(VPATH)
OBJS_common = string_utils.o test_asserter.o
OBJS_mc6809 = registers_hd6309.o table_mc6809.o
OBJS_hd6809 = $(OBJS_mc6809) table_hd6309.o
OBJS_r6502  = table_r6502.o
OBJS_r65c02 = $(OBJS_r6502) table_r65c02.o
OBJS_i8080  = registers_i8080.o  table_i8080.o
OBJS_tms9995= table_tms9995.o
OBJS_z80    = registers_z80.o    table_z80.o
OBJS        = \
	$(OBJS_common) \
	$(OBJS_hd6309) \
	$(OBJS_r65c02) \
	asm_i8080.o   dis_i8080.o   $(OBJS_i8080) \
	asm_tms9995.o dis_tms9995.o $(OBJS_tms9995) \
	asm_z80.o     dis_z80.o     $(OBJS_Z80) \

test_asm_mc6809: test_asm_mc6809.o $(OBJS_common) $(OBJS_mc6809)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_mc6809: test_dis_mc6809.o $(OBJS_common) $(OBJS_mc6809)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_hd6309: test_asm_hd6309.o $(OBJS_common) $(OBJS_hd6309)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_hd6309: test_dis_hd6309.o $(OBJS_common) $(OBJS_hd6309)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_r6502:  test_asm_r6502.o  $(OBJS_common) $(OBJS_r6502.o)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_r6502:  test_dis_r6502.o  $(OBJS_common) $(OBJS_r6502.o)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_r65c02: test_asm_r65c02.o $(OBJS_common) $(OBJS_r65c02.o)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_r65c02: test_dis_r65c02.o $(OBJS_common) $(OBJS_r65c02.o)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_i8080:  test_asm_i8080.o  $(OBJS_common) asm_i8080.o $(OBJS_i8080)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_i8080:  test_dis_i8080.o  $(OBJS_common) dis_i8080.o $(OBJS_i8080)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_tms9995: test_asm_tms9995.o $(OBJS_common) asm_tms9995.o $(OBJS_tms9995)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_tms9995: test_dis_tms9995.o $(OBJS_common) dis_tms9995.o $(OBJS_tms9995)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_z80:    test_asm_z80.o $(OBJS_common) asm_z80.o $(OBJS_z80)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_z80  :  test_dis_z80.o $(OBJS_common) dis_z80.o $(OBJS_z80)
	$(CXX) -o $@ $(CXXFLAGS) $^

TEST_ASM_HEADERS=test_asm_helper.h test_asserter.h test_symtab.h
TEST_DIS_HEADERS=test_dis_helper.h test_asserter.h test_symtab.h test_memory.h
test_asm_mc6809.o:  $(TEST_ASM_HEADERS) asm_hd6309.h
test_dis_mc6809.o:  $(TEST_DIS_HEADERS) dis_hd6309.h
test_asm_hd6309.o:  $(TEST_ASM_HEADERS) asm_hd6309.h
test_dis_hd6309.o:  $(TEST_DIS_HEADERS) dis_hd6309.h
test_asm_r6502.o:   $(TEST_ASM_HEADERS) asm_r65c02.h
test_dis_r6502.o:   $(TEST_DIS_HEADERS) dis_r65c02.h
test_asm_r65c02.o:  $(TEST_ASM_HEADERS) asm_r65c02.h
test_dis_r65c02.o:  $(TEST_DIS_HEADERS) dis_r65c02.h
test_asm_i8080.o:   $(TEST_ASM_HEADERS) asm_i8080.h
test_dis_i8080.o:   $(TEST_DIS_HEADERS) dis_i8080.h
test_asm_tms9995.o: $(TEST_ASM_HEADERS) asm_tms9995.h
test_dis_tms9995.o: $(TEST_DIS_HEADERS) dis_tms9995.h
test_asm_z80.o:     $(TEST_ASM_HEADERS) asm_z80.h
test_dis_z80.o:     $(TEST_DIS_HEADERS) dis_z80.h

test_asserter.o: test_asserter.h
test_memory.h: memory.h
test_symtab.h: symbol_table.h

clean: clean-obj
	-rm -f $(TESTS)
clean-obj:
	-rm -f *.o
