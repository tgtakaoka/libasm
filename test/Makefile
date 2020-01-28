VPATH=../src

ASL_FLAGS = -gnuerrors -L +t 0x1e -q

all: \
	test_mc6809.s19 \
	test_hd6309.s19 \
	test_m6502.s19 \
	test_w65c02.s19 \
	test_tms9900.hex \
	test_tms9995.hex \
	test_i8080.hex \
	test_z80.hex \

%.hex: %.asm
	asl $(ASL_FLAGS) $^
	p2hex -r \$$-\$$ -l 32 -F Intel $*.p $@ > /dev/null
%.s19: %.asm
	asl $(ASL_FLAGS) $^
	p2hex -r \$$-\$$ -l 32 -F Moto +5 $*.p $@ > /dev/null

.PHONY: clean clean-obj test \
	test_mc6809  \
	test_m6502   \
	test_i8080   \
	test_z80     \
	test_tms9900 \
	test_expr

test:   test_mc6809  \
	test_m6502   \
	test_i8080   \
        test_z80     \
	test_tms9900 \
	test_expr

test_mc6809: test_asm_mc6809 test_dis_mc6809
	./test_asm_mc6809
	./test_dis_mc6809
test_m6502: test_asm_m6502 test_dis_m6502
	./test_asm_m6502
	./test_dis_m6502
test_i8080:  test_asm_i8080  test_dis_i8080
	./test_asm_i8080
	./test_dis_i8080
test_tms9900:test_asm_tms9900 test_dis_tms9900
	./test_asm_tms9900
	./test_dis_tms9900
test_z80: test_asm_z80 test_dis_z80
	./test_asm_z80
	./test_dis_z80
test_expr: test_expr_moto test_expr_intel test_expr_cstyle
	./test_expr_moto
	./test_expr_intel
	./test_expr_cstyle

CXXFLAGS    = -Wall -g -std=c++17 -O -I$(VPATH)
CPPFLAGS    = -MMD

TEST_OBJS= \
	test_asm_mc6809.o  test_dis_mc6809.o  \
	test_asm_m6502.o   test_dis_m6502.o   \
	test_asm_i8080.o   test_dis_i8080.o   \
	test_asm_z80.o     test_dis_z80.o     \
	test_asm_tms9900.o test_dis_tms9900.o \
	test_expr_moto.o   test_expr_intel.o  \
	test_expr_cstyle.o

TESTS=$(TEST_OBJS:.o=)

OBJS_common  = test_asserter.o asm_operand.o dis_operand.o
OBJS_mc6809  = table_mc6809.o  reg_mc6809.o
OBJS_m6502   = table_m6502.o   reg_m6502.o
OBJS_i8080   = table_i8080.o   reg_i8080.o
OBJS_z80     = table_z80.o     reg_z80.o
OBJS_tms9900 = table_tms9900.o reg_tms9900.o
OBJS         = \
	$(TEST_OBJS) \
	$(OBJS_common) \
	$(OBJS_mc6809)  asm_mc6809.o  dis_mc6809.o  \
	$(OBJS_m6502)   asm_m6502.o   dis_m6502.o   \
	$(OBJS_i8080)   asm_i8080.o   dis_i8080.o   \
	$(OBJS_Z80)     asm_z80.o     dis_z80.o     \
	$(OBJS_tms9900) asm_tms9900.o dis_tms9900.o

-include $(OBJS:.o=.d) $(TEST_OBJS:.o=.d)

test_asm_mc6809: test_asm_mc6809.o   asm_mc6809.o  $(OBJS_mc6809)  $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_mc6809: test_dis_mc6809.o   dis_mc6809.o  $(OBJS_mc6809)  $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_m6502:  test_asm_m6502.o    asm_m6502.o   $(OBJS_m6502)   $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_m6502:  test_dis_m6502.o    dis_m6502.o   $(OBJS_m6502)   $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_i8080:  test_asm_i8080.o    asm_i8080.o   $(OBJS_i8080)   $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_i8080:  test_dis_i8080.o    dis_i8080.o   $(OBJS_i8080)   $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_z80:    test_asm_z80.o      asm_z80.o     $(OBJS_z80)     $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_z80:    test_dis_z80.o      dis_z80.o     $(OBJS_z80)     $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_tms9900: test_asm_tms9900.o asm_tms9900.o $(OBJS_tms9900) $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_tms9900: test_dis_tms9900.o dis_tms9900.o $(OBJS_tms9900) $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_expr_moto:  test_expr_moto.o  $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_expr_intel: test_expr_intel.o $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_expr_cstyle:test_expr_cstyle.o $(OBJS_common)
	$(CXX) -o $@ $(CXXFLAGS) $^

clean: clean-obj
	-rm -f $(TESTS)
clean-obj:
	-rm -f *.o *.d *.p *.lst
