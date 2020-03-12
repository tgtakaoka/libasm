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

CXXFLAGS=-Wall -g -O -std=c++17
CPPFLAGS=-MMD

OBJS_mc6809  = asm_mc6809.o  dis_mc6809.o  table_mc6809.o  reg_mc6809.o
OBJS_mc6800  = asm_mc6800.o  dis_mc6800.o  table_mc6800.o  reg_mc6800.o
OBJS_m6502   = asm_m6502.o   dis_m6502.o   table_m6502.o   reg_m6502.o
OBJS_i8080   = asm_i8080.o   dis_i8080.o   table_i8080.o   reg_i8080.o
OBJS_tms9900 = asm_tms9900.o dis_tms9900.o table_tms9900.o reg_tms9900.o
OBJS_z80     = asm_z80.o     dis_z80.o     table_z80.o     reg_z80.o
OBJS_mc68000 = asm_mc68000.o dis_mc68000.o table_mc68000.o reg_mc68000.o
OBJS = \
	$(OBJS_mc6809)  \
	$(OBJS_mc6800)  \
	$(OBJS_m6502)   \
	$(OBJS_i8080)   \
	$(OBJS_z80)     \
	$(OBJS_tms9900) \
	$(OBJS_mc68000)

.PHONY: all objs clean test

all: objs
objs: $(OBJS)

-include $(OBJS:.o=.d)

clean:
	-rm -f $(OBJS) $(OBJS:.o=.d)

# Local Variables:
# mode: makefile-gmake
# End:
# vim: set ft=make:
