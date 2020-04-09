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

OBJS_common   = file_util.o
OBJS_asm      = asm_operand.o
OBJS_dis      = dis_operand.o
OBJS_mc6809   = table_mc6809.o  reg_mc6809.o  $(OBJS_common)
OBJS_mc6800   = table_mc6800.o  reg_mc6800.o  $(OBJS_common)
OBJS_m6502    = table_m6502.o   reg_m6502.o   $(OBJS_common)
OBJS_i8080    = table_i8080.o   reg_i8080.o   $(OBJS_common)
OBJS_z80      = table_z80.o     reg_z80.o     $(OBJS_common)
OBJS_tms9900  = table_tms9900.o reg_tms9900.o $(OBJS_common)
OBJS_mc68000  = table_mc68000.o reg_mc68000.o $(OBJS_common)
OBJS_asm6809  = asm6809.o  asm_mc6809.o  $(OBJS_asm) $(OBJS_mc6809)  $(OBJS_common)
OBJS_asm6800  = asm6800.o  asm_mc6800.o  $(OBJS_asm) $(OBJS_mc6800)  $(OBJS_common)
OBJS_asm6502  = asm6502.o  asm_m6502.o   $(OBJS_asm) $(OBJS_m6502)   $(OBJS_common)
OBJS_asm8080  = asm8080.o  asm_i8080.o   $(OBJS_asm) $(OBJS_i8080)   $(OBJS_common)
OBJS_asmz80   = asmz80.o   asm_z80.o     $(OBJS_asm) $(OBJS_z80)     $(OBJS_common)
OBJS_asm9900  = asm9900.o  asm_tms9900.o $(OBJS_asm) $(OBJS_tms9900) $(OBJS_common)
OBJS_asm68000 = asm68000.o asm_mc68000.o $(OBJS_asm) $(OBJS_mc68000) $(OBJS_common)
OBJS_dis6809  = dis6809.o  dis_mc6809.o  $(OBJS_dis) $(OBJS_mc6809)  $(OBJS_common)
OBJS_dis6800  = dis6800.o  dis_mc6800.o  $(OBJS_dis) $(OBJS_mc6800)  $(OBJS_common)
OBJS_dis6502  = dis6502.o  dis_m6502.o   $(OBJS_dis) $(OBJS_m6502)   $(OBJS_common)
OBJS_dis8080  = dis8080.o  dis_i8080.o   $(OBJS_dis) $(OBJS_i8080)   $(OBJS_common)
OBJS_disz80   = disz80.o   dis_z80.o     $(OBJS_dis) $(OBJS_z80)     $(OBJS_common)
OBJS_dis9900  = dis9900.o  dis_tms9900.o $(OBJS_dis) $(OBJS_tms9900) $(OBJS_common)
OBJS_dis68000 = dis68000.o dis_mc68000.o $(OBJS_dis) $(OBJS_mc68000) $(OBJS_common)

OBJS = \
	$(OBJS_asm6809)  $(OBJS_dis6809)  \
	$(OBJS_asm6800)  $(OBJS_dis6800)  \
	$(OBJS_asm6502)  $(OBJS_dis6502)  \
	$(OBJS_asm8080)  $(OBJS_dis8080)  \
	$(OBJS_asmz80)   $(OBJS_disz80)   \
	$(OBJS_asm9900)  $(OBJS_dis9900)  \
	$(OBJS_asm68000) $(OBJS_dis68000)

OBJS_driver = \
	asm6809.o  \
	asm6800.o  \
	asm6502.o  \
	asm8080.o  \
	asmz80.o   \
	asm9900.o  \
	asm68000.o \
	dis6809.o  \
	dis6800.o  \
	dis6502.o  \
	dis8080.o  \
	disz80.o   \
	dis9900.o  \
	dis68000.o

DRIVERS = $(OBJS_driver:.o=)

.PHONY: all clean distclean

all: $(OBJS_driver:.o=)

-include $(OBJS:.o=.d)

asm6809:  $(OBJS_asm6809)
	$(CXX) $(CXXFLAGS) $^ -o $@
asm6800:  $(OBJS_asm6800)
	$(CXX) $(CXXFLAGS) $^ -o $@
asm6502:  $(OBJS_asm6502)
	$(CXX) $(CXXFLAGS) $^ -o $@
asm8080:  $(OBJS_asm8080)
	$(CXX) $(CXXFLAGS) $^ -o $@
asmz80:   $(OBJS_asmz80)
	$(CXX) $(CXXFLAGS) $^ -o $@
asm9900:  $(OBJS_asm9900)
	$(CXX) $(CXXFLAGS) $^ -o $@
asm68000: $(OBJS_asm68000)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis6809:  $(OBJS_dis6809)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis6800:  $(OBJS_dis6800)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis6502:  $(OBJS_dis6502)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis8080:  $(OBJS_dis8080)
	$(CXX) $(CXXFLAGS) $^ -o $@
disz80:   $(OBJS_disz80)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis9900:  $(OBJS_dis9900)
	$(CXX) $(CXXFLAGS) $^ -o $@
dis68000: $(OBJS_dis68000)
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
