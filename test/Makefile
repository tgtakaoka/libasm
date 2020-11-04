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

ASL_FLAGS = -gnuerrors -L +t 0x1e -q -w

TEST_OUT=\
	test_mc6809.s19 \
	test_hd6309.s19 \
	test_mc6800.s19 \
	test_mc6801.s19 \
	test_hd6301.s19 \
	test_mc68hc11.s19 \
	test_m6811.s19 \
	test_mos6502.s19 \
	test_w65sc02.s19 \
	test_r65c02.s19 \
	test_w65c02s.s19 \
	test_w65c816_m1x1.s28 \
	test_w65c816_m0x0.s28 \
	test_w65c816_m1x0.s28 \
	test_w65c816_m0x1.s28 \
	test_i8051.hex \
	test_i8080.hex \
	test_i8085.hex \
	test_i8080_z80syn.hex \
	test_i8085_z80syn.hex \
	test_z80.hex \
	test_z8.hex \
	test_z86c.hex \
	test_z88.hex \
	test_ins8060.hex \
	test_ins8070.hex \
	test_cdp1802.hex \
	test_i8086.hex \
	test_i386.hex \
	test_tms9900.hex \
	test_tms9995.hex \
	test_tms99105.hex \
	test_mc68000.s28 \
	test_m68k.s28 \
	test_z8001.hex \
	test_z8002.hex \
	test_ns32000.hex

all: $(TEST_OUT)

%.hex: %.asl
	asl $(ASL_FLAGS) $^
	p2hex -k -r \$$-\$$ -l 32 -F Intel $*.p $@ > /dev/null
%.s19: %.asl
	asl $(ASL_FLAGS) $^
	p2hex -k -r \$$-\$$ -l 32 -F Moto +5 $*.p $@ > /dev/null
%.s28: %.asl
	asl $(ASL_FLAGS) $^
	p2hex -k -r \$$-\$$ -l 32 -F Moto +5 $*.p $@ > /dev/null
%.s37: %.asl
	asl $(ASL_FLAGS) $^
	p2hex -k -r \$$-\$$ -l 32 -F Moto +5 $*.p $@ > /dev/null
test_z8001.hex: test_z8001.gas
	z8k-coff-as --warn -a=test_z8001.lst -o test_z8001.p $^
	z8k-coff-ld -m z8001 --entry=0 -o test_z8001.o test_z8001.p \
		--section-start=text=0x010000 \
		--section-start=bss1=0x0A1234 \
		--section-start=bss2=0x0B5678
	z8k-coff-objcopy -I coff-z8k -O ihex test_z8001.o /dev/stdout \
		--adjust-section-vma .text=0x01000 | \
		srec_cat /dev/stdin -intel -HEAder "" -output $@ -Intel -line-length=76
#	z8k-coff-objdump -m z8001 -D test_z8001.hex
test_z8002.hex: test_z8002.gas
	z8k-coff-as --warn -a=test_z8002.lst -o test_z8002.p $^
	z8k-coff-ld -m z8002 --entry=0 -o test_z8002.o test_z8002.p \
		--section-start=text=0x1000 \
		--section-start=bss1=0x1234 \
		--section-start=bss2=0x5678
	z8k-coff-objcopy -I coff-z8k -O ihex test_z8002.o /dev/stdout \
		--adjust-section-vma .text=0x1000 | \
		srec_cat /dev/stdin -intel -HEAder "" -output $@ -Intel -line-length=76
#	z8k-coff-objdump -m z8002 -D test_z8002.hex
test_ns32000.hex: test_ns32000.gas
	ns32k-netbsd-as -m32032 --warn -a=test_ns32000.lst -o test_ns32000.o $^
	ns32k-netbsd-objcopy -I a.out-ns32k-netbsd -O ihex -j .text \
		--adjust-section-vma .text=0x10000 \
		test_ns32000.o /dev/stdout | \
		srec_cat /dev/stdin -intel -HEAder "" -output $@ -Intel -line-length=76
#	ns32k-netbsd-objdump -m ns32k:32032 -D test_ns32000.hex
test_i386.hex: test_i386.gas
	i386-elf-as -mtune=i8086 --warn -mnaked-reg -msyntax=intel -mmnemonic=intel \
		-a=test_i386.lst -o test_i386.p $^
	i386-elf-ld --entry=0 -o test_i386.o test_i386.p \
		--section-start=.text=0x1000
	i386-elf-objcopy -I elf32-i386 -O ihex test_i386.o /dev/stdout | \
		srec_cat /dev/stdin -intel -output $@ -Intel -line-length=76
#	i386-elf-objdump -m i8086 -M intel -D test_i386.hex
test_m68k.s28: test_m68k.gas
	m68k-elf-as -m68000 --warn --register-prefix-optional --base-size-default-16 \
		-a=test_m68k.lst -o test_m68k.p $^
	m68k-elf-ld --entry=0 -o test_m68k.o test_m68k.p
	m68k-elf-objcopy -I elf32-m68k -O srec --srec-len 32 \
		--adjust-section-vma .text=0x10000 \
		test_m68k.o /dev/stdout | \
		srec_cat /dev/stdin -HEAder "" -output $@
#	m68k-elf-objdump -m 68000 -D test_mc68000.s28
test_m6811.s19: test_m6811.gas
	m6811-elf-as -m68hc11 --warn -a=test_m6811.lst -o test_m6811.p $^
	m6811-elf-ld -m m68hc11elf --entry=0 -o test_m6811.o test_m6811.p
	m6811-elf-objcopy -I elf32-m68hc11 -O srec --srec-len 32 \
		--adjust-section-vma .text=0x0100 \
		test_m6811.o /dev/stdout | \
		srec_cat /dev/stdin -HEAder "" -output $@
#	m6811-elf-objdump -m m68hc11 -D test_m6811.s19

.PHONY: clean clean-obj test \
	test_mc6809  \
	test_mc6800  \
	test_mos6502 \
	test_i8080   \
	test_z80     \
	test_z8      \
	test_ins8060 \
	test_ins8070 \
	test_cdp1802 \
	test_i8086   \
	test_tms9900 \
	test_mc68000 \
	test_z8000   \
	test_ns32000 \
	test_expr

test:   test_mc6809  \
	test_mc6800  \
	test_mos6502 \
	test_i8051   \
	test_i8080   \
	test_z80     \
	test_z8      \
	test_ins8060 \
	test_ins8070 \
	test_cdp1802 \
	test_i8086   \
	test_tms9900 \
	test_mc68000 \
	test_z8000   \
	test_ns32000 \
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
test_i8051:  test_asm_i8051 test_dis_i8051
	./test_asm_i8051
	./test_dis_i8051
test_i8080:  test_asm_i8080  test_dis_i8080
	./test_asm_i8080
	./test_dis_i8080
test_z80: test_asm_z80 test_dis_z80
	./test_asm_z80
	./test_dis_z80
test_z8: test_asm_z8 test_dis_z8
	./test_asm_z8
	./test_dis_z8
test_ins8060: test_asm_ins8060 test_dis_ins8060
	./test_asm_ins8060
	./test_dis_ins8060
test_ins8070: test_asm_ins8070 test_dis_ins8070
	./test_asm_ins8070
	./test_dis_ins8070
test_cdp1802: test_asm_cdp1802 test_dis_cdp1802
	./test_asm_cdp1802
	./test_dis_cdp1802
test_i8086: test_asm_i8086 test_dis_i8086
	./test_asm_i8086
	./test_dis_i8086
test_tms9900:test_asm_tms9900 test_dis_tms9900
	./test_asm_tms9900
	./test_dis_tms9900
test_mc68000: test_asm_mc68000 test_dis_mc68000
	./test_asm_mc68000
	./test_dis_mc68000
test_z8000: test_asm_z8000 test_dis_z8000
	./test_asm_z8000
	./test_dis_z8000
test_ns32000: test_asm_ns32000 test_dis_ns32000
	./test_asm_ns32000
	./test_dis_ns32000
test_expr: test_expr_moto test_expr_intel test_expr_base
	./test_expr_moto
	./test_expr_intel
	./test_expr_base

gen: \
	gen_mc6809  \
	gen_mc6800  \
	gen_mos6502 \
	gen_i8051   \
	gen_i8080   \
	gen_z80     \
	gen_z8      \
	gen_ins8060 \
	gen_ins8070 \
	gen_cdp1802 \
	gen_i8086   \
	gen_tms9900 \
	gen_mc68000 \
	gen_z8000

CXXFLAGS    = -Wall -g -std=c++14 -O $(INCLUDES) $(DEBUG_FLAGS)
CPPFLAGS    = -MMD

TEST_OBJS= \
	test_asm_mc6809.o  test_dis_mc6809.o  \
	test_asm_mc6800.o  test_dis_mc6800.o  \
	test_asm_mos6502.o test_dis_mos6502.o \
	test_asm_i8051.o   test_dis_i8051.o   \
	test_asm_i8080.o   test_dis_i8080.o   \
	test_asm_z80.o     test_dis_z80.o     \
	test_asm_z8.o      test_dis_z8.o      \
	test_asm_ins8060.o test_dis_ins8060.o \
	test_asm_ins8070.o test_dis_ins8070.o \
	test_asm_cdp1802.o test_dis_cdp1802.o \
	test_asm_i8086.o   test_dis_i8086.o   \
	test_asm_tms9900.o test_dis_tms9900.o \
	test_asm_mc68000.o test_dis_mc68000.o \
	test_asm_z8000.o   test_dis_z8000.o   \
	test_asm_ns32000.o test_dis_ns32000.o \
	test_expr_moto.o   test_expr_intel.o  \
	test_expr_base.o \
	gen_mc6809.o  \
	gen_mc6800.o  \
	gen_mos6502.o \
	gen_i8051.o   \
	gen_i8080.o   \
	gen_z80.o     \
	gen_z8.o      \
	gen_ins8060.o \
	gen_ins8070.o \
	gen_cdp1802.o \
	gen_i8086.o   \
	gen_tms9900.o \
	gen_mc68000.o \
	gen_z8000.o

TESTS=$(TEST_OBJS:.o=)

OBJS_test = test_asserter.o error_reporter.o
OBJS_asm  = asm_base.o test_asm_helper.o value_parser.o
OBJS_dis  = dis_base.o test_dis_helper.o value_formatter.o test_memory.o
OBJS_expr = test_expr_helper.o test_asserter.o error_reporter.o \
            value_parser.o value_formatter.o
OBJS_gen  = dis_base.o value_formatter.o test_memory.o text_buffer.o cli_listing.o
OBJS_mc6809  = table_mc6809.o  reg_mc6809.o
OBJS_mc6800  = table_mc6800.o  reg_mc6800.o
OBJS_mos6502 = table_mos6502.o reg_mos6502.o
OBJS_i8051   = table_i8051.o   reg_i8051.o
OBJS_i8080   = table_i8080.o   reg_i8080.o
OBJS_z80     = table_z80.o     reg_z80.o
OBJS_z8      = table_z8.o      reg_z8.o
OBJS_ins8060 = table_ins8060.o reg_ins8060.o
OBJS_ins8070 = table_ins8070.o reg_ins8070.o
OBJS_cdp1802 = table_cdp1802.o
OBJS_i8086   = table_i8086.o   reg_i8086.o
OBJS_tms9900 = table_tms9900.o reg_tms9900.o
OBJS_mc68000 = table_mc68000.o reg_mc68000.o
OBJS_z8000   = table_z8000.o   reg_z8000.o
OBJS_ns32000 = table_ns32000.o reg_ns32000.o
OBJS	 = \
	$(TEST_OBJS) $(OBJS_test) $(OBJS_asm) $(OBJS_dis) $(OBJS_gen) \
	$(OBJS_mc6809)  asm_mc6809.o  dis_mc6809.o  \
	$(OBJS_mc6800)  asm_mc6800.o  dis_mc6800.o  \
	$(OBJS_mos6502) asm_mos6502.o dis_mos6502.o \
	$(OBJS_i8051)   asm_i8051.o   dis_i8051.o   \
	$(OBJS_i8080)   asm_i8080.o   dis_i8080.o   \
	$(OBJS_z80)     asm_z80.o     dis_z80.o     \
	$(OBJS_z8)      asm_z8.o      dis_z8.o      \
	$(OBJS_ins8060) asm_ins8060.o dis_ins8060.o \
	$(OBJS_ins8070) asm_ins8070.o dis_ins8070.o \
	$(OBJS_cdp1802) asm_cdp1802.o dis_cdp1802.o \
	$(OBJS_i8086)   asm_i8086.o   dis_i8086.o   \
	$(OBJS_tms9900) asm_tms9900.o dis_tms9900.o \
	$(OBJS_mc68000) asm_mc68000.o dis_mc68000.o \
	$(OBJS_z8000)   asm_z8000.o   dis_z8000.o   \
	$(OBJS_ns32000) asm_ns32000.o dis_ns32000.o

-include $(OBJS:.o=.d) $(TEST_OBJS:.o=.d)

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

gen_mc6809:  gen_mc6809.o  dis_mc6809.o  $(OBJS_mc6809)  $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_mc6809 -u -C 6809 -o gen_mc6809.asm
	./gen_mc6809 -u -C 6309 -o gen_hd6309.asm
gen_mc6800:  gen_mc6800.o  dis_mc6800.o  $(OBJS_mc6800)  $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_mc6800 -u -C 6800 -o gen_mc6800.asm
	./gen_mc6800 -u -C 6801 -o gen_mc6801.asm
	./gen_mc6800 -u -C 6301 -o gen_hd6301.asm
	./gen_mc6800 -u -C 6811 -o gen_mc68hc11.asm
gen_mos6502: gen_mos6502.o dis_mos6502.o $(OBJS_mos6502) $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_mos6502 -u -C 6502    -o gen_mos6502.asm
	./gen_mos6502 -u -C 65SC02  -o gen_w65sc02.asm
	./gen_mos6502 -u -C 65C02   -o gen_r65c02.asm
	./gen_mos6502 -u -C W65C02S -o gen_w65c02s.asm
	./gen_mos6502 -u -C 65816   -o gen_w65c816.asm
gen_i8051:   gen_i8051.o   dis_i8051.o   $(OBJS_i8051)   $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_i8051 -u -C 8051 -o gen_i8051.asm
gen_i8080:   gen_i8080.o   dis_i8080.o   $(OBJS_i8080)   $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_i8080 -u -C 8080 -o gen_i8080.asm
	./gen_i8080 -u -C 8085 -o gen_i8085.asm
gen_z80:     gen_z80.o     dis_z80.o     $(OBJS_z80)     $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_z80   -u -C Z80  -o gen_z80.asm
	./gen_z80   -u -C 8080 -o gen_i8080_z80syn.asm
	./gen_z80   -u -C 8085 -o gen_i8085_z80syn.asm
gen_z8:      gen_z8.o      dis_z8.o      $(OBJS_z8)      $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_z8    -u -C Z8601  -o gen_z8.asm
	./gen_z8    -u -C Z86C40 -o gen_z86c40.asm
	./gen_z8    -u -C Z88C00 -o gen_z88.asm
gen_ins8060: gen_ins8060.o dis_ins8060.o $(OBJS_ins8060) $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_ins8060 -u -C SC/MP -o gen_ins8060.asm
gen_ins8070: gen_ins8070.o dis_ins8070.o $(OBJS_ins8070) $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_ins8070 -u -C 8070 -o gen_ins8070.asm
gen_cdp1802: gen_cdp1802.o dis_cdp1802.o $(OBJS_cdp1802) $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_cdp1802 -u -C 1802 -o gen_cdp1802.asm
gen_i8086:   gen_i8086.o   dis_i8086.o   $(OBJS_i8086)   $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_i8086   -u -C 8086  -o gen_i8086.asm
gen_tms9900: gen_tms9900.o dis_tms9900.o $(OBJS_tms9900) $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_tms9900 -u -C TMS9900  -o gen_tms9900.asm
	./gen_tms9900 -u -C TMS9995  -o gen_tms9995.asm
	./gen_tms9900 -u -C TMS99105 -o gen_tms99105.asm
gen_mc68000: gen_mc68000.o dis_mc68000.o $(OBJS_mc68000) $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_mc68000 -u -C 68000 -o gen_mc68000.asm
gen_z8000:   gen_z8000.o   dis_z8000.o   $(OBJS_z8000)   $(OBJS_gen)
	$(CXX) -o $@ $(CXXFLAGS) $^
	./gen_z8000 -C z8001 -c -o gen_z8001.inc
	./gen_z8000 -C z8002 -c -o gen_z8002.inc

clean: clean-obj
	-rm -f $(TESTS)
clean-obj:
	-rm -f *.o *.d *.p *.lst

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
