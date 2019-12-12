VPATH=../src

.PHONY: clean clean-obj test \
	test_mc6809 test_hd6309 \
	test_r6502 test_r65c02 \
	test_tms9995 \
	test_i8080 test_z80

test:   test_mc6809 test_hd6309 \
	test_r6502 test_r65c02 \
	test_tms9995 \
	test_i8080 test_z80

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

CXXFLAGS    = -Wall -g -std=c++17 -O -I$(VPATH)
CPPFLAGS    = -MMD

TEST_OBJS= \
	test_asm_mc6809.o  test_asm_hd6309.o \
	test_dis_mc6809.o  test_dis_hd6309.o \
	test_asm_r6502.o   test_dis_r6502.o \
	test_asm_r65c02.o  test_dis_r65c02.o \
	test_asm_i8080.o   test_dis_i8080.o \
	test_asm_tms9995.o test_dis_tms9995.o \
	test_asm_z80.o     test_dis_z80.o
TESTS=$(TEST_OBJS:.o=)

OBJS_common = dis_operand.o test_asserter.o
OBJS_mc6809 = table_mc6809.o  reg_hd6309.o
OBJS_hd6309 = table_hd6309.o  $(OBJS_mc6809)
OBJS_r6502  = table_r6502.o
OBJS_r65c02 = table_r65c02.o  $(OBJS_r6502)
OBJS_i8080  = table_i8080.o   reg_i8080.o
OBJS_tms9995= table_tms9995.o
OBJS_z80    = table_z80.o     reg_z80.o
OBJS        = \
	$(TEST_OBJS) \
	$(OBJS_common) \
	$(OBJS_hd6309) \
	$(OBJS_r65c02) \
	$(OBJS_i8080)   asm_i8080.o   dis_i8080.o \
	$(OBJS_tms9995) asm_tms9995.o dis_tms9995.o \
	$(OBJS_Z80)     asm_z80.o     dis_z80.o

-include $(OBJS:.o=.d) $(TEST_OBJS:.o=.d)

test_asm_mc6809: test_asm_mc6809.o $(OBJS_mc6809) $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_mc6809: test_dis_mc6809.o $(OBJS_mc6809) $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_hd6309: test_asm_hd6309.o $(OBJS_hd6309) $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_hd6309: test_dis_hd6309.o $(OBJS_hd6309) $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_r6502:  test_asm_r6502.o  $(OBJS_r6502)  $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_r6502:  test_dis_r6502.o  $(OBJS_r6502)  $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_r65c02: test_asm_r65c02.o $(OBJS_r65c02) $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_r65c02: test_dis_r65c02.o $(OBJS_r65c02) $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_i8080:  test_asm_i8080.o    asm_i8080.o   $(OBJS_i8080)   $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_i8080:  test_dis_i8080.o    dis_i8080.o   $(OBJS_i8080)   $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_tms9995: test_asm_tms9995.o asm_tms9995.o $(OBJS_tms9995) $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_tms9995: test_dis_tms9995.o dis_tms9995.o $(OBJS_tms9995) $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_z80:    test_asm_z80.o      asm_z80.o     $(OBJS_z80)     $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_z80  :  test_dis_z80.o      dis_z80.o     $(OBJS_z80)     $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^

clean: clean-obj
	-rm -f $(TESTS)
clean-obj:
	-rm -f *.o *.d
