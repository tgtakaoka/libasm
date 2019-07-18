
CXXFLAGS=-Wall -g -O -std=c++17

OBJS_6809 = asm_6809.o table_6809.o
OBJS_6309 = asm_6309.o table_6309.o

.PHONY: clean-objs clean

all: $(OBJS_6809) $(OBJS_6309)

asm_6809.o: asm_6809.h string_utils.h
asm_6309.o: asm_6309.h string_utils.h
string_utils.o: string_utils.h
table_6809.o: config_6809.h insn_entry.h text_6809.h
table_6309.o: config_6309.h insn_entry.h text_6309.h

asm_6809.h: config_6309.h insn.h table_6809.h
asm_6309.h: config_6809.h asm_6809.h table_6809.h
config_6809.h: config_host.h
config_6309.h: config_host.h
error_reporter.h : config_host.h
insn.h: error_reporter.h config_host.h config_target.h symbol_table.h
insn_entry.h: config_host.h config_target.h
insn_table.h: config_host.h config_target.h error_reporter.h
string_utils.h: config_host.h config_target.h
symbol_table.h: config_host.h config_target.h
table.h: config_host.h config_target.h
text_6809.h: config_host.h
text_6309.h: config_host.h

clean-objs:
	-rm -f $(OBJS_6809) $(OBJS_6309)

clean: clean-objs

