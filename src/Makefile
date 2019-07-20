
CXXFLAGS=-Wall -g -O -std=c++17

OBJS_mc6809 = asm_mc6809.o dis_mc6809.o table_mc6809.o string_utils.o
OBJS_hd6309 = asm_hd6309.o dis_hd6309.o table_hd6309.o string_utils.o

.PHONY: clean-objs clean test

all: $(OBJS_mc6809) $(OBJS_hd6309)

asm_mc6809.o: asm_mc6809.h string_utils.h
asm_hd6309.o: asm_hd6309.h string_utils.h
dis_mc6809.o: dis_mc6809.h string_utils.h table_mc6809.h
dis_hd6309.o: dis_hd6309.h string_utils.h table_hd6309.h
string_utils.o: config_host.h string_utils.h
table_mc6809.o: config_mc6809.h table_mc6809.h text_mc6809.h
table_hd6309.o: config_hd6309.h table_hd6309.h text_hd6309.h

asm_mc6809.h: config_mc6809.h error_reporter.h symbol_table.h table_mc6809.h
asm_hd6309.h: config_hd6309.h asm_mc6809.h table_mc6809.h
config_mc6809.h: config_host.h error_mc6809.h
config_hd6309.h: config_host.h config_mc6809.h
config_host.h:
dis_mc6809.h: config_mc6809.h error_reporter.h memory.h symbol_table.h
dis_hd6309.h: config_hd6309.h dis_mc6809.h
entry_mc6809.h: config_host.h
error_reporter.h : config_host.h
insn_mc6809.h:
memory.h:
string_utils.h:
symbol_table.h: config_host.h
table_mc6809.h: config_mc6809.h entry_mc6809.h
table_hd6309.h: table_mc6809.h
text_mc6809.h: config_host.h
text_hd6309.h: config_host.h

clean-objs:
	-rm -f $(OBJS_mc6809) $(OBJS_hd6309)

clean: clean-objs

