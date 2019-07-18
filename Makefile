
CXXFLAGS=-Wall -g -O -std=c++17

OBJS_mc6809 = asm_mc6809.o table_mc6809.o
OBJS_hd6309 = asm_hd6309.o table_hd6309.o

.PHONY: clean-objs clean test

all: $(OBJS_mc6809) $(OBJS_hd6309)

asm_mc6809.o: asm_mc6809.h string_utils.h
asm_hd6309.o: asm_hd6309.h string_utils.h
string_utils.o: string_utils.h
table_mc6809.o: config_mc6809.h insn_entry.h text_mc6809.h
table_hd6309.o: config_hd6309.h insn_entry.h text_hd6309.h

asm_mc6809.h: config_hd6309.h insn.h table_mc6809.h
asm_hd6309.h: config_mc6809.h asm_mc6809.h table_mc6809.h
config_mc6809.h: config_host.h
config_hd6309.h: config_host.h
error_reporter.h : config_host.h
insn.h: error_reporter.h config_host.h config_target.h symbol_table.h
insn_entry.h: config_host.h config_target.h
insn_table.h: config_host.h config_target.h error_reporter.h
string_utils.h: config_host.h config_target.h
symbol_table.h: config_host.h config_target.h
table.h: config_host.h config_target.h
text_mc6809.h: config_host.h
text_hd6309.h: config_host.h

clean-objs:
	-rm -f $(OBJS_mc6809) $(OBJS_hd6309)

clean: clean-objs

