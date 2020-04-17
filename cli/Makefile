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

INSTALL_DIR ?= /usr/local/bin
VPATH=../src
CXXFLAGS=-Wall -g -O -std=c++17 -I../src
CPPFLAGS=-MMD

OBJS_common   = file_util.o bin_formatter.o intel_hex.o moto_srec.o cli_listing.o cli_memory.o
OBJS_asm      = asm_base.o asm_operand.o asm_driver.o asm_directive.o
OBJS_dis      = dis_base.o dis_operand.o dis_driver.o dis_directive.o
OBJS_mc6809   = table_mc6809.o  reg_mc6809.o
OBJS_mc6800   = table_mc6800.o  reg_mc6800.o
OBJS_mos6502  = table_mos6502.o reg_mos6502.o
OBJS_i8080    = table_i8080.o   reg_i8080.o
OBJS_z80      = table_z80.o     reg_z80.o
OBJS_tms9900  = table_tms9900.o reg_tms9900.o
OBJS_mc68000  = table_mc68000.o reg_mc68000.o

OBJS = 	$(OBJS_common)  \
	$(OBJS_asm)     \
	$(OBJS_dis)     \
	$(OBJS_mc6809)  \
	$(OBJS_mc6800)  \
	$(OBJS_mos6502) \
	$(OBJS_i8080)   \
	$(OBJS_z80)     \
	$(OBJS_tms9900) \
	$(OBJS_mc68000)

OBJS_driver = asm.o dis.o

DRIVERS = $(OBJS_driver:.o=)

.PHONY: all clean distclean

all: $(DRIVERS)

-include $(OBJS:.o=.d)

asm:    asm.o $(OBJS_asm) $(OBJS_common) \
	asm_mc6809.o  $(OBJS_mc6809)  \
	asm_mc6800.o  $(OBJS_mc6800)  \
	asm_mos6502.o $(OBJS_mos6502) \
	asm_i8080.o   $(OBJS_i8080)   \
	asm_z80.o     $(OBJS_z80)     \
	asm_tms9900.o $(OBJS_tms9900) \
	asm_mc68000.o $(OBJS_mc68000)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis:    dis.o $(OBJS_dis) $(OBJS_common) \
	dis_mc6809.o  $(OBJS_mc6809)  \
	dis_mc6800.o  $(OBJS_mc6800)  \
	dis_mos6502.o $(OBJS_mos6502) \
	dis_i8080.o   $(OBJS_i8080)   \
	dis_z80.o     $(OBJS_z80)     \
	dis_tms9900.o $(OBJS_tms9900) \
	dis_mc68000.o $(OBJS_mc68000)
	$(CXX) $(CXXFLAGS) $^ -o $@

install: $(DRIVERS)
	install $(DRIVERS) ${INSTALL_DIR}
clean:
	-rm -f *.o *.d
distclean: clean
	-rm -f $(DRIVERS)

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
