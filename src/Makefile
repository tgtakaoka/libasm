
CXXFLAGS=-Wall -g -O -std=c++17

OBJS_mc6809 = asm_mc6809.o dis_mc6809.o table_mc6809.o string_utils.o
OBJS_hd6309 = asm_hd6309.o dis_hd6309.o table_hd6309.o string_utils.o
OBJS_i8080 = dis_i8080.o table_i8080.o

.PHONY: clean-objs clean test

all: $(OBJS_mc6809) $(OBJS_hd6309) $(OBJS_i8080)

asm_mc6809.o: asm_mc6809.h
asm_hd6309.o: asm_hd6309.h
dis_mc6809.o: dis_mc6809.h string_utils.h table_mc6809.h
dis_hd6309.o: dis_hd6309.h string_utils.h table_hd6309.h
string_utils.o: config_host.h string_utils.h
table_mc6809.o: config_mc6809.h table_mc6809.h text_mc6809.h
table_hd6309.o: config_hd6309.h table_hd6309.h text_hd6309.h
table_i8080.o: config_i8080.h entry_i8080.h table_i8080.h text_i8080.h

asm_mc6809.h: config_mc6809.h error_reporter.h symbol_table.h table_mc6809.h
asm_hd6309.h: config_hd6309.h asm_mc6809.h table_mc6809.h
config_mc6809.h: config_host.h error_mc6809.h
config_hd6309.h: config_host.h config_mc6809.h
config_host.h:
dis_mc6809.h: config_mc6809.h error_reporter.h memory.h symbol_table.h
dis_hd6309.h: config_hd6309.h dis_mc6809.h
entry_mc6809.h: config_host.h
entry_i8080.h: config_host.h
error_reporter.h : config_host.h
insn_mc6809.h:
insn_i8080.h: config_i8080.h entry_i8080.h
memory.h:
string_utils.h:
symbol_table.h: config_host.h
table_mc6809.h: config_mc6809.h entry_mc6809.h
table_hd6309.h: table_mc6809.h
table_i8080.h: insn_i8080.h
text_mc6809.h: config_host.h
text_hd6309.h: config_host.h

clean-objs:
	-rm -f $(OBJS_mc6809) $(OBJS_hd6309) $(OBJS_i8080)

clean: clean-objs
