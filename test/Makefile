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

all:

INCLUDES=-I ../src -I ../cli
VPATH=../src:../cli
CXXFLAGS    = -Wall -std=c++14 -O $(INCLUDES) $(DEBUG_FLAGS)
CPPFLAGS    = -MMD

ARCHS = mc6809 mc6800 mos6502 i8080 z80 z8 tlcs90 ins80560 ins8070 cdp1802 \
	i8086 tms9900 mc68000 z8000 ns32000
CPUS_mc6809  = 6809 6309
CPUS_mc6800  = 6800 6801 6811 6301
CPUS_mos6502 = 6502 65SC02 65C02 W65C02S 65816
CPUS_i8080   = 8080 8085
CPUS_z80     = Z80 8080 8085
CPUS_z8      = Z8601 Z86C40 Z88C00
CPUS_tlcs90  = TLCS90
CPUS_ins8060 = SC/MP
CPUS_ins8070 = 8070
CPUS_cdp1802 = 1802
CPUS_i8086   = 8086
CPUS_tms9900 = TMS9900 TMS9995 TMS99105
CPUS_mc68000 = 68000
CPUS_z8000   = Z8001 Z8002
CPUS_ns32000 = 32032

TESTS= $(TEST_OBJS:.o=)
TEST_OBJS = $(ARCHS:%=test_asm_%.o) $(ARCHS:%=test_dis_%.o) \
	test_expr_moto.o test_expr_intel.o test_expr_base.o

.PHONY: all clean clean-objs test $(TESTS) gen generate

OBJS_test = test_asserter.o error_reporter.o
OBJS_asm  = asm_base.o test_asm_helper.o value_parser.o
OBJS_dis  = dis_base.o test_dis_helper.o value_formatter.o test_memory.o
OBJS_expr = test_expr_helper.o test_asserter.o error_reporter.o \
            value_parser.o value_formatter.o
OBJS_mc6809  = table_mc6809.o  reg_mc6809.o
OBJS_mc6800  = table_mc6800.o  reg_mc6800.o
OBJS_mos6502 = table_mos6502.o reg_mos6502.o
OBJS_i8051   = table_i8051.o   reg_i8051.o
OBJS_i8080   = table_i8080.o   reg_i8080.o
OBJS_z80     = table_z80.o     reg_z80.o
OBJS_z8      = table_z8.o      reg_z8.o
OBJS_tlcs90  = table_tlcs90.o  reg_tlcs90.o
OBJS_ins8060 = table_ins8060.o reg_ins8060.o
OBJS_ins8070 = table_ins8070.o reg_ins8070.o
OBJS_cdp1802 = table_cdp1802.o
OBJS_i8086   = table_i8086.o   reg_i8086.o
OBJS_tms9900 = table_tms9900.o reg_tms9900.o
OBJS_mc68000 = table_mc68000.o reg_mc68000.o
OBJS_z8000   = table_z8000.o   reg_z8000.o
OBJS_ns32000 = table_ns32000.o reg_ns32000.o
OBJS = 	$(TEST_OBJS) $(OBJS_test) $(OBJS_asm) $(OBJS_dis) $(OBJS_gen) \
	$(foreach a,$(ARCHS),$(OBJS_$a)) \
	$(ARCHS:%=asm_%.o) $(ARCHS:%=dis_%.o)

-include $(OBJS:.o=.d)

test: $(TESTS)
	@for t in $(TESTS); do \
	    echo ./$${t}; \
	    ./$${t}; \
	done

test_asm_mc6809:  test_asm_mc6809.o  asm_mc6809.o  $(OBJS_mc6809)  $(OBJS_asm) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_mc6809:  test_dis_mc6809.o  dis_mc6809.o  $(OBJS_mc6809)  $(OBJS_dis) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_mc6800:  test_asm_mc6800.o  asm_mc6800.o  $(OBJS_mc6800)  $(OBJS_asm) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_mc6800:  test_dis_mc6800.o  dis_mc6800.o  $(OBJS_mc6800)  $(OBJS_dis) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_mos6502: test_asm_mos6502.o asm_mos6502.o $(OBJS_mos6502) $(OBJS_asm) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_mos6502: test_dis_mos6502.o dis_mos6502.o $(OBJS_mos6502) $(OBJS_dis) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_i8051:   test_asm_i8051.o   asm_i8051.o   $(OBJS_i8051)   $(OBJS_asm) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_i8051:   test_dis_i8051.o   dis_i8051.o   $(OBJS_i8051)   $(OBJS_dis) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_i8080:   test_asm_i8080.o   asm_i8080.o   $(OBJS_i8080)   $(OBJS_asm) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_i8080:   test_dis_i8080.o   dis_i8080.o   $(OBJS_i8080)   $(OBJS_dis) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_z80:     test_asm_z80.o     asm_z80.o     $(OBJS_z80)     $(OBJS_asm) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_z80:     test_dis_z80.o     dis_z80.o     $(OBJS_z80)     $(OBJS_dis) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_z8:      test_asm_z8.o      asm_z8.o      $(OBJS_z8)      $(OBJS_asm) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_z8:      test_dis_z8.o      dis_z8.o      $(OBJS_z8)      $(OBJS_dis) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_tlcs90:  test_asm_tlcs90.o  asm_tlcs90.o  $(OBJS_tlcs90)  $(OBJS_asm) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_tlcs90:  test_dis_tlcs90.o  dis_tlcs90.o  $(OBJS_tlcs90)  $(OBJS_dis) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_ins8060: test_asm_ins8060.o asm_ins8060.o $(OBJS_ins8060) $(OBJS_asm) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_ins8060: test_dis_ins8060.o dis_ins8060.o $(OBJS_ins8060) $(OBJS_dis) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_ins8070: test_asm_ins8070.o asm_ins8070.o $(OBJS_ins8070) $(OBJS_asm) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_ins8070: test_dis_ins8070.o dis_ins8070.o $(OBJS_ins8070) $(OBJS_dis) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_cdp1802: test_asm_cdp1802.o asm_cdp1802.o $(OBJS_cdp1802) $(OBJS_asm) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_cdp1802: test_dis_cdp1802.o dis_cdp1802.o $(OBJS_cdp1802) $(OBJS_dis) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_i8086:   test_asm_i8086.o   asm_i8086.o   $(OBJS_i8086)   $(OBJS_asm) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_i8086:   test_dis_i8086.o   dis_i8086.o   $(OBJS_i8086)   $(OBJS_dis) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_tms9900: test_asm_tms9900.o asm_tms9900.o $(OBJS_tms9900) $(OBJS_asm) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_tms9900: test_dis_tms9900.o dis_tms9900.o $(OBJS_tms9900) $(OBJS_dis) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_mc68000: test_asm_mc68000.o asm_mc68000.o $(OBJS_mc68000) $(OBJS_asm) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_mc68000: test_dis_mc68000.o dis_mc68000.o $(OBJS_mc68000) $(OBJS_dis) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_z8000:   test_asm_z8000.o   asm_z8000.o   $(OBJS_z8000)   $(OBJS_asm) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_z8000:   test_dis_z8000.o   dis_z8000.o   $(OBJS_z8000)   $(OBJS_dis) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_asm_ns32000: test_asm_ns32000.o asm_ns32000.o $(OBJS_ns32000) $(OBJS_asm) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_dis_ns32000: test_dis_ns32000.o dis_ns32000.o $(OBJS_ns32000) $(OBJS_dis) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_expr_moto:   test_expr_moto.o  $(OBJS_expr) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_expr_intel:  test_expr_intel.o $(OBJS_expr) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^
test_expr_base:   test_expr_base.o  $(OBJS_expr) $(OBJS_test)
	$(CXX) -o $@ $(CXXFLAGS) $^

GENS=$(GEN_OBJS:.o=)
GEN_OBJS=\
	gen_mc6809.o  \
	gen_mc6800.o  \
	gen_mos6502.o \
	gen_i8051.o   \
	gen_i8080.o   \
	gen_z80.o     \
	gen_z8.o      \
	gen_tlcs90.o  \
	gen_ins8060.o \
	gen_ins8070.o \
	gen_cdp1802.o \
	gen_i8086.o   \
	gen_tms9900.o \
	gen_mc68000.o \
	gen_z8000.o   \
	gen_ns32000.o

GEN_OUTS=\
	gen_mc6809.asm  gen_hd6309.asm \
	gen_mc6800.asm  gen_mc6801.asm  gen_hd6309.asm gen_mc68hc11.asm \
	gen_mos6502.asm gen_w65sc02.asm gen_r65c02.asm gen_w65c02s.asm  gen_w65c816.asm \
	gen_i8051.asm  \
	gen_i8080.asm   gen_i8085.asm \
	gen_z80.asm  gen_i8080_z80syn.asm  gen_i8085_z80syn.asm \
	gen_z8.asm   gen_z86c40.asm  gen_z88.asm \
	gen_tlcs90.inc  \
	gen_ins8060.asm \
	gen_ins8070.asm \
	gen_cdp1802.asm \
	gen_i8086.asm   \
	gen_tms9900.asm gen_tms9995.asm gen_tms99105.asm \
	gen_mc68000.asm \
	gen_z8001.asm  gen_z8002.asm  gen_z8001.inc  gen_z8002.inc \
	gen_ns32032.inc

OBJS_gen = dis_base.o value_formatter.o test_memory.o text_buffer.o \
	cli_listing.o error_reporter.o

gen: $(GENS)

generate: $(GEN_OUTS)

asm-gen:
	@for s in $(GEN_OUTS); do \
	    x=$${s%.asm}; x=$${x%.inc}; \
	    echo asm $$x.asm -o $$x.hex -l $$x.lst; \
	    asm $$x.asm -H32 -o $$x.hex -l $$x.lst; \
	done
asl-gen:
	@for s in $(GEN_OUTS); do \
	    x=$${s%.asm}; x=$${x%.inc}; \
	    a=$$x.asm; [ -f $$x.asl ] && a=$$x.asl; \
	    [ $$x = gen_i8086 ] && continue; \
	    [ $$x = gen_ns32032 ] && continue; \
	    echo "asl $(ASL_FLAGS) $$a && p2hex $(P2HEX_FLAGS) -F Intel $$x.p $$x.hex"; \
	    asl $(ASL_FLAGS) $$a && p2hex $(P2HEX_FLAGS) -F Intel $$x.p $$x.hex; \
	done
GAS_GENS =  gen_z8k1.gas gen_z8k2.gas gen_ns32k.gas
gas-gen: clean-gas-gen $(GAS_GENS:.gas=.hex)
clean-gas-gen:
	-rm -f $(GAS_GENS:.gas=.hex)

clean-gen:
	-rm -f $(GEN_OUTS)

gen_mc6809:  gen_mc6809.o  dis_mc6809.o  $(OBJS_mc6809)  $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
gen_mc6800:  gen_mc6800.o  dis_mc6800.o  $(OBJS_mc6800)  $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
gen_mos6502: gen_mos6502.o dis_mos6502.o $(OBJS_mos6502) $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
gen_i8051:   gen_i8051.o   dis_i8051.o   $(OBJS_i8051)   $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
gen_i8080:   gen_i8080.o   dis_i8080.o   $(OBJS_i8080)   $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
gen_z80:     gen_z80.o     dis_z80.o     $(OBJS_z80)     $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
gen_z8:      gen_z8.o      dis_z8.o      $(OBJS_z8)      $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
gen_tlcs90:  gen_tlcs90.o  dis_tlcs90.o  $(OBJS_tlcs90)  $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
gen_ins8060: gen_ins8060.o dis_ins8060.o $(OBJS_ins8060) $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
gen_ins8070: gen_ins8070.o dis_ins8070.o $(OBJS_ins8070) $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
gen_cdp1802: gen_cdp1802.o dis_cdp1802.o $(OBJS_cdp1802) $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
gen_i8086:   gen_i8086.o   dis_i8086.o   $(OBJS_i8086)   $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
gen_tms9900: gen_tms9900.o dis_tms9900.o $(OBJS_tms9900) $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
gen_mc68000: gen_mc68000.o dis_mc68000.o $(OBJS_mc68000) $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
gen_z8000:   gen_z8000.o   dis_z8000.o   $(OBJS_z8000)   $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
gen_ns32000: gen_ns32000.o dis_ns32000.o $(OBJS_ns32000) $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^

gen_mc6809.asm: gen_mc6809
	./$^ -u -C 6809 -o $@ -l $(@:.asm=.lst)
gen_hd6309.asm: gen_mc6809
	./$^ -u -C 6309 -o $@ -l $(@:.asm=.lst)
gen_mc6800.asm: gen_mc6800
	./$^ -u -C 6800 -o $@ -l $(@:.asm=.lst)
gen_mc6801.asm: gen_mc6800
	./$^ -u -C 6801 -o $@ -l $(@:.asm=.lst)
gen_hd6301.asm: gen_mc6800
	./$^ -u -C 6301 -o $@ -l $(@:.asm=.lst)
gen_mc68hc11.asm: gen_mc6800
	./$^ -u -C 6811 -o $@ -l $(@:.asm=.lst)
gen_mos6502.asm: gen_mos6502
	./$^ -u -C 6502 -o $@ -l $(@:.asm=.lst)
gen_w65sc02.asm: gen_mos6502
	./$^ -u -C 65SC02 -o $@ -l $(@:.asm=.lst)
gen_r65c02.asm: gen_mos6502
	./$^ -u -C 65C02 -o $@ -l $(@:.asm=.lst)
gen_w65c02s.asm: gen_mos6502
	./$^ -u -C W65C02S -o $@ -l $(@:.asm=.lst)
gen_w65c816.asm: gen_mos6502
	./$^ -u -C 65816 -o $@ -l $(@:.asm=.lst)
gen_i8051.asm: gen_i8051
	./$^ -u -C 8051 -o $@ -l $(@:.asm=.lst)
gen_i8080.asm: gen_i8080
	./$^ -u -C 8080 -o $@ -l $(@:.asm=.lst)
gen_i8085.asm: gen_i8080
	./$^ -u -C 8085 -o $@ -l $(@:.asm=.lst)
gen_z80.asm: gen_z80
	./$^ -u -C Z80 -o $@ -l $(@:.asm=.lst)
gen_i8080_z80syn.asm: gen_z80
	./$^ -u -C 8080 -o $@ -l $(@:.asm=.lst)
gen_i8085_z80syn.asm: gen_z80
	./$^ -u -C 8085 -o $@ -l $(@:.asm=.lst)
gen_z8.asm: gen_z8
	./$^ -u -C Z8601 -o $@ -l $(@:.asm=.lst)
gen_z86c40.asm: gen_z8
	./$^ -u -C Z86C40 -o $@ -l $(@:.asm=.lst)
gen_z88.asm: gen_z8
	./$^ -u -C Z88C00 -o $@ -l $(@:.asm=.lst)
gen_tlcs90.inc: gen_tlcs90
	./$^ -c -u -C TLCS90 -o $@ -l $(@:.inc=.lst)
gen_ins8060.asm: gen_ins8060
	./$^ -u -C SC/MP -o $@ -l $(@:.asm=.lst)
gen_ins8070.asm: gen_ins8070
	./$^ -u -C 8070 -o $@ -l $(@:.asm=.lst)
gen_cdp1802.asm: gen_cdp1802
	./$^ -u -C 1802 -o $@ -l $(@:.asm=.lst)
gen_i8086.asm: gen_i8086
	./$^ -u -C 8086 -o $@ -l $(@:.asm=.lst)
gen_tms9900.asm: gen_tms9900
	./$^ -u -C TMS9900 -o $@ -l $(@:.asm=.lst)
gen_tms9995.asm: gen_tms9900
	./$^ -u -C TMS9995 -o $@ -l $(@:.asm=.lst)
gen_tms99105.asm: gen_tms9900
	./$^ -u -C TMS99105 -o $@ -l $(@:.asm=.lst)
gen_mc68000.asm: gen_mc68000
	./$^ -u -C 68000 -o $@ -l $(@:.asm=.lst)
gen_z8001.asm: gen_z8000
	./$^ -u -C z8001 -o $@ -l $(@:.asm=.lst)
gen_z8002.asm: gen_z8000
	./$^ -u -C z8002 -o $@ -l $(@:.asm=.lst)
gen_z8001.inc: gen_z8000
	./$^ -g -C z8001 -o $@ -l $(@:.inc=.lst)
gen_z8002.inc: gen_z8000
	./$^ -g -C z8002 -o $@ -l $(@:.inc=.lst)
gen_ns32032.inc: gen_ns32000
	./$^ -g -C 32032 -o $@ -l $(@:.inc=.lst)

clean: clean-objs
	-rm -f $(TESTS) $(GENS)
clean-objs:
	-rm -f *.o *.d *.p *.lst *.out

ASL_FLAGS = -gnuerrors -L +t 0x1e -q -w -relaxed
P2HEX_FLAGS = -q -k -r \$$-\$$ -l 32

asm-test:
	@for s in test_*.asm; do \
	    x=$${s%.asm}; o=$$x.hex; \
	    [ -f $$x.s19 ] && o=$$x.s19; \
	    [ -f $$x.s28 ] && o=$$x.s28; \
	    echo asm $$s -o $$o -l $${x}.lst; \
	    asm $$s -o $$o -l $${x}.lst; \
	done

asl-test:
	@for s in test_*.asl; do \
	    x=$${s%.asl}; o=$$x.hex; f=Intel; \
	    [ -f $$x.s19 ] && { o=$$x.s19; f="Moto +5"; }; \
	    [ -f $$x.s28 ] && { o=$$x.s28; f="Moto +5"; }; \
	    echo asl $(ASL_FLAGS) $$s; \
	    echo p2hex $(P2HEX_FLAGS) -F $$f $$x.p $$o; \
	    asl $(ASL_FLAGS) $$s; \
	    p2hex $(P2HEX_FLAGS) -F $$f $$x.p $$o; \
	done

GAS_TESTS = test_z8k1.hex test_z8k2.hex test_ns32k.hex test_i386.hex test_m68k.s28 test_m6811.s19
gas-test: clean-gas $(GAS_TESTS)
clean-gas:
	-rm -f $(GAS_TESTS)

clean-test:
	-rm -f test_*.hex test_*.s19 test_*.s28

%.hex: %.asl
	asl $(ASL_FLAGS) $^
	p2hex -q -k -r \$$-\$$ -l 32 -F Intel $*.p $@
%.s19: %.asl
	asl $(ASL_FLAGS) $^
	p2hex -q -k -r \$$-\$$ -l 32 -F Moto +5 $*.p $@
%.s28: %.asl
	asl $(ASL_FLAGS) $^
	p2hex -q -k -r \$$-\$$ -l 32 -F Moto +5 $*.p $@
%.s37: %.asl
	asl $(ASL_FLAGS) $^
	p2hex -q -k -r \$$-\$$ -l 32 -F Moto +5 $*.p $@
%_z8k1.hex: %_z8k1.gas
	z8k-coff-as --warn -o $(@:.hex=.p) $^
	z8k-coff-ld -m z8001 --entry=0 -o $(@:.hex=.o) $(@:.hex=.p) \
		--section-start=text=0x010000 \
		--section-start=bss1=0x0A1234 \
		--section-start=bss2=0x0B5678
	z8k-coff-objcopy -I coff-z8k -O ihex $(@:.hex=.o) /dev/stdout \
		--adjust-section-vma .text=0x010000 | \
		srec_cat /dev/stdin -intel -HEAder "" -output $@ -Intel -line-length=76
	z8k-coff-objdump -m z8001 -D $@ > $(@:.hex=.lst)
%_z8k2.hex: %_z8k2.gas
	z8k-coff-as --warn -o $(@:.hex=.p) $^
	z8k-coff-ld -m z8002 --entry=0 -o $(@:.hex=.o) $(@:.hex=.p) \
		--section-start=text=0x1000 \
		--section-start=bss1=0x1234 \
		--section-start=bss2=0x5678
	z8k-coff-objcopy -I coff-z8k -O ihex $(@:.hex=.o) /dev/stdout \
		--adjust-section-vma .text=0x1000 | \
		srec_cat /dev/stdin -intel -HEAder "" -output $@ -Intel -line-length=76
	z8k-coff-objdump -m z8002 -D $@ > $(@:.hex=.lst)
%_ns32k.hex: %_ns32k.gas
	ns32k-netbsd-as -m32032 --warn -o $(@:.hex=.o) $^
	ns32k-netbsd-objcopy -I a.out-ns32k-netbsd -O ihex -j .text \
		--adjust-section-vma .text=0x10000 $(@:.hex=.o) /dev/stdout | \
		srec_cat /dev/stdin -intel -HEAder "" -output $@ -Intel -line-length=76
	ns32k-netbsd-objdump -m ns32k:32032 -D $@ > $(@:.hex=.lst)
%_i386.hex: %_i386.gas
	i386-elf-as -mtune=i8086 --warn -mnaked-reg -msyntax=intel -mmnemonic=intel -o $(@:.hex=.p) $^
	i386-elf-ld --entry=0 -section-start=.text=0x1000 -o $(@:.hex=.o) $(@:.hex=.p)
	i386-elf-objcopy -I elf32-i386 -O ihex $(@:.hex=.o) /dev/stdout | \
		srec_cat /dev/stdin -intel -output $@ -Intel -line-length=76
	i386-elf-objdump -m i8086 -M intel -D $@ > $(@:.hex=.lst)
%_m68k.s28: %_m68k.gas
	m68k-elf-as -m68000 --warn --register-prefix-optional --base-size-default-16 -o $(@:.s28=.p) $^
	m68k-elf-ld --entry=0 -o $(@:.s28=.o) $(@:.s28=.p)
	m68k-elf-objcopy -I elf32-m68k -O srec --srec-len 32 \
		--adjust-section-vma .text=0x10000 $(@:.s28=.o) /dev/stdout | \
		srec_cat /dev/stdin -HEAder "" -output $@
	m68k-elf-objdump -m 68000 -D $@ > $(@:.s28=.lst)
%_m6811.s19: %_m6811.gas
	m6811-elf-as -m68hc11 --warn -o $(@:.s19=.p) $^
	m6811-elf-ld -m m68hc11elf --entry=0 -o $(@:.s19=.o) $(@:.s19=.p)
	m6811-elf-objcopy -I elf32-m68hc11 -O srec --srec-len 32 \
		--adjust-section-vma .text=0x0100 $(@:.s19=.o) /dev/stdout | \
		srec_cat /dev/stdin -HEAder "" -output $@
	m6811-elf-objdump -m m68hc11 -D $@ > $(@:.s19=.lst)

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
