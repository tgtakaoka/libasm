# Copyright 2020 Tadashi G. Takaoka
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

INCLUDES=-I ../src -I ../cli
VPATH=../src:../cli

ASL_FLAGS = -gnuerrors -L +t 0x1e -q

all: \
	test_mc6809.s19 \
	test_hd6309.s19 \
	test_mc6800.s19 \
	test_mc6801.s19 \
	test_hd6301.s19 \
	test_mos6502.s19 \
	test_w65sc02.s19 \
	test_r65c02.s19 \
	test_w65c02s.s19 \
	test_w65c816.s28 \
	test_i8080.hex \
	test_i8085.hex \
	test_i8080_z80syn.hex \
	test_i8085_z80syn.hex \
	test_z80.hex \
	test_ins8060.hex \
	test_ins8070.hex \
	test_cdp1802.hex \
	test_tms9900.hex \
	test_tms9995.hex \
	test_mc68000.s28

%.hex: %.asm
	asl $(ASL_FLAGS) $^
	p2hex -r \$$-\$$ -l 32 -F Intel $*.p $@ > /dev/null
%.s19: %.asm
	asl $(ASL_FLAGS) $^
	p2hex -r \$$-\$$ -l 32 -F Moto +5 $*.p $@ > /dev/null
%.s28: %.asm
	asl $(ASL_FLAGS) $^
	p2hex -r \$$-\$$ -l 32 -F Moto +5 $*.p $@ > /dev/null
%.s37: %.asm
	asl $(ASL_FLAGS) $^
	p2hex -r \$$-\$$ -l 32 -F Moto +5 $*.p $@ > /dev/null

.PHONY: clean clean-obj test \
	test_mc6809  \
	test_mc6800  \
	test_mos6502 \
	test_w65c816 \
	test_i8080   \
	test_z80     \
	test_ins8060 \
	test_ins8070 \
	test_cdp1802 \
	test_tms9900 \
	test_mc68000 \
	test_expr

test:   test_mc6809  \
	test_mc6800  \
	test_mos6502 \
	test_w65c816 \
	test_i8080   \
	test_z80     \
	test_ins8060 \
	test_ins8070 \
	test_cdp1802 \
	test_tms9900 \
	test_mc68000 \
	test_expr

test_mc6809: test_asm_mc6809 test_dis_mc6809
	./test_asm_mc6809
	./test_dis_mc6809
test_mc6800: test_asm_mc6800 test_dis_mc6800
	./test_asm_mc6800
	./test_dis_mc6800
test_mos6502: test_asm_mos6502 test_dis_mos6502
	./test_asm_mos6502
	./test_dis_mos6502
test_w65c816: test_asm_w65c816 test_dis_w65c816
	./test_asm_w65c816
	./test_dis_w65c816
test_i8080:  test_asm_i8080  test_dis_i8080
	./test_asm_i8080
	./test_dis_i8080
test_tms9900:test_asm_tms9900 test_dis_tms9900
	./test_asm_tms9900
	./test_dis_tms9900
test_z80: test_asm_z80 test_dis_z80
	./test_asm_z80
	./test_dis_z80
test_ins8060: test_asm_ins8060 test_dis_ins8060
	./test_asm_ins8060
	./test_dis_ins8060
test_ins8070: test_asm_ins8070 test_dis_ins8070
	./test_asm_ins8070
	./test_dis_ins8070
test_cdp1802: test_asm_cdp1802 test_dis_cdp1802
	./test_asm_cdp1802
	./test_dis_cdp1802
test_mc68000: test_asm_mc68000 test_dis_mc68000
	./test_asm_mc68000
	./test_dis_mc68000
test_expr: test_expr_moto test_expr_intel test_expr_cstyle
	./test_expr_moto
	./test_expr_intel
	./test_expr_cstyle

gen: \
	gen_mc6809  \
	gen_mc6800  \
	gen_mos6502 \
	gen_i8080   \
	gen_z80     \
	gen_ins8060

CXXFLAGS    = -Wall -g -std=c++17 -O $(INCLUDES) $(DEBUG_FLAGS)
CPPFLAGS    = -MMD

TEST_OBJS= \
	test_asm_mc6809.o  test_dis_mc6809.o  \
	test_asm_mc6800.o  test_dis_mc6800.o  \
	test_asm_mos6502.o test_dis_mos6502.o \
	test_asm_w65c816.o test_dis_w65c816.o \
	test_asm_i8080.o   test_dis_i8080.o   \
	test_asm_z80.o     test_dis_z80.o     \
	test_asm_ins8060.o test_dis_ins8060.o \
	test_asm_ins8070.o test_dis_ins8070.o \
	test_asm_cdp1802.o test_dis_cdp1802.o \
	test_asm_tms9900.o test_dis_tms9900.o \
	test_asm_mc68000.o test_dis_mc68000.o \
	test_expr_moto.o   test_expr_intel.o  \
	test_expr_cstyle.o \
	gen_mc6809.o  \
	gen_mc6800.o  \
	gen_mos6502.o \
	gen_i8080.o   \
	gen_z80.o     \
	gen_ins8060.o \
	gen_ins8070.o \
	gen_cdp1802.o

TESTS=$(TEST_OBJS:.o=)

OBJS_asm = asm_base.o asm_operand.o test_asserter.o
OBJS_dis = dis_base.o dis_operand.o test_asserter.o test_memory.o
OBJS_gen = dis_base.o dis_operand.o test_memory.o text_buffer.o cli_listing.o
OBJS_mc6809  = table_mc6809.o  reg_mc6809.o
OBJS_mc6800  = table_mc6800.o  reg_mc6800.o
OBJS_mos6502 = table_mos6502.o reg_mos6502.o
OBJS_w65c816 = table_w65c816.o reg_mos6502.o table_mos6502.o
OBJS_i8080   = table_i8080.o   reg_i8080.o
OBJS_z80     = table_z80.o     reg_z80.o
OBJS_ins8060 = table_ins8060.o reg_ins8060.o
OBJS_ins8070 = table_ins8070.o reg_ins8070.o
OBJS_cdp1802 = table_cdp1802.o
OBJS_tms9900 = table_tms9900.o reg_tms9900.o
OBJS_mc68000 = table_mc68000.o reg_mc68000.o
OBJS	 = \
	$(TEST_OBJS) (OBJS_asm) $(OBJS_dis) $(OBJS_gen) \
	$(OBJS_mc6809)  asm_mc6809.o  dis_mc6809.o  \
	$(OBJS_mc6800)  asm_mc6800.o  dis_mc6800.o  \
	$(OBJS_mos6502) asm_mos6502.o dis_mos6502.o \
	$(OBJS_w65c816) asm_w65c816.o dis_w65c816.o \
	$(OBJS_i8080)   asm_i8080.o   dis_i8080.o   \
	$(OBJS_Z80)     asm_z80.o     dis_z80.o     \
	$(OBJS_ins8060) asm_ins8060.o dis_ins8060.o \
	$(OBJS_ins8070) asm_ins8070.o dis_ins8070.o \
	$(OBJS_cdp1802) asm_cdp1802.o dis_cdp1802.o \
	$(OBJS_tms9900) asm_tms9900.o dis_tms9900.o \
	$(OBJS_mc68000) asm_mc68000.o dis_mc68000.o

-include $(OBJS:.o=.d) $(TEST_OBJS:.o=.d)

test_asm_mc6809:  test_asm_mc6809.o  asm_mc6809.o  $(OBJS_mc6809)  $(OBJS_asm)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_mc6809:  test_dis_mc6809.o  dis_mc6809.o  $(OBJS_mc6809)  $(OBJS_dis)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_mc6800:  test_asm_mc6800.o  asm_mc6800.o  $(OBJS_mc6800)  $(OBJS_asm)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_mc6800:  test_dis_mc6800.o  dis_mc6800.o  $(OBJS_mc6800)  $(OBJS_dis)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_mos6502: test_asm_mos6502.o asm_mos6502.o $(OBJS_mos6502) $(OBJS_asm)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_mos6502: test_dis_mos6502.o dis_mos6502.o $(OBJS_mos6502) $(OBJS_dis)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_w65c816: test_asm_w65c816.o asm_w65c816.o $(OBJS_w65c816) $(OBJS_asm)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_w65c816: test_dis_w65c816.o dis_w65c816.o $(OBJS_w65c816) $(OBJS_dis)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_i8080:   test_asm_i8080.o   asm_i8080.o   $(OBJS_i8080)   $(OBJS_asm)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_i8080:   test_dis_i8080.o   dis_i8080.o   $(OBJS_i8080)   $(OBJS_dis)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_z80:     test_asm_z80.o     asm_z80.o     $(OBJS_z80)     $(OBJS_asm)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_z80:     test_dis_z80.o     dis_z80.o     $(OBJS_z80)     $(OBJS_dis)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_ins8060: test_asm_ins8060.o asm_ins8060.o $(OBJS_ins8060) $(OBJS_asm)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_ins8060: test_dis_ins8060.o dis_ins8060.o $(OBJS_ins8060) $(OBJS_dis)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_ins8070: test_asm_ins8070.o asm_ins8070.o $(OBJS_ins8070) $(OBJS_asm)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_ins8070: test_dis_ins8070.o dis_ins8070.o $(OBJS_ins8070) $(OBJS_dis)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_cdp1802: test_asm_cdp1802.o asm_cdp1802.o $(OBJS_cdp1802) $(OBJS_asm)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_cdp1802: test_dis_cdp1802.o dis_cdp1802.o $(OBJS_cdp1802) $(OBJS_dis)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_tms9900: test_asm_tms9900.o asm_tms9900.o $(OBJS_tms9900) $(OBJS_asm)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_tms9900: test_dis_tms9900.o dis_tms9900.o $(OBJS_tms9900) $(OBJS_dis)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_mc68000: test_asm_mc68000.o asm_mc68000.o $(OBJS_mc68000) $(OBJS_asm)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_mc68000: test_dis_mc68000.o dis_mc68000.o $(OBJS_mc68000) $(OBJS_dis)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_expr_moto:  test_expr_moto.o    $(OBJS_asm) $(OBJS_dis)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_expr_intel:  test_expr_intel.o  $(OBJS_asm) $(OBJS_dis)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_expr_cstyle: test_expr_cstyle.o $(OBJS_asm) $(OBJS_dis)
	$(CXX) -o $@ $(CXXFLAGS) $^

gen_mc6809:  gen_mc6809.o  dis_mc6809.o  $(OBJS_mc6809)  $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_mc6809 -u -C mc6809 -o gen_mc6809.asm
	./gen_mc6809 -u -C hd6309 -o gen_hd6309.asm
gen_mc6800:  gen_mc6800.o  dis_mc6800.o  $(OBJS_mc6800)  $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_mc6800 -u -C mc6800  -o gen_mc6800.asm
	./gen_mc6800 -u -C mc6801  -o gen_mc6801.asm
	./gen_mc6800 -u -C hd6301  -o gen_hd6301.asm
gen_mos6502: gen_mos6502.o dis_mos6502.o $(OBJS_mos6502) $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_mos6502 -u -C mos6502 -o gen_mos6502.asm
	./gen_mos6502 -u -C w65sc02 -o gen_w65sc02.asm
	./gen_mos6502 -u -C r65c02  -o gen_r65c02.asm
	./gen_mos6502 -u -C w65c02s -o gen_w65c02s.asm
gen_i8080:   gen_i8080.o   dis_i8080.o   $(OBJS_i8080)   $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_i8080 -u -C i8080 -o gen_i8080.asm
	./gen_i8080 -u -C i8085 -o gen_i8085.asm
gen_z80:     gen_z80.o     dis_z80.o     $(OBJS_z80)     $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_z80   -u -C z80   -o gen_z80.asm
	./gen_z80   -u -C i8080 -o gen_i8080_z80syn.asm
	./gen_z80   -u -C i8085 -o gen_i8085_z80syn.asm
gen_ins8060: gen_ins8060.o dis_ins8060.o $(OBJS_ins8060) $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_ins8060 -u -C ins8060 -o gen_ins8060.asm
gen_ins8070: gen_ins8070.o dis_ins8070.o $(OBJS_ins8070) $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_ins8070 -u -C ins8070 -o gen_ins8070.asm
gen_cdp1802: gen_cdp1802.o dis_cdp1802.o $(OBJS_cdp1802) $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_cdp1802 -u -C cdp1802 -o gen_cdp1802.asm

clean: clean-obj
	-rm -f $(TESTS)
clean-obj:
	-rm -f *.o *.d *.p *.lst

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
